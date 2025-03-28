import sys
import hid
import psutil
import wmi
import time
import os

# -------------------- YOUR ORIGINAL CODE --------------------
# HID Device Configuration
vendor_id = 0x6E61
product_id = 0x6063
usage_page = 0xFF60
usage = 0x61
report_length = 32  # Adjust based on QMK setup

def get_raw_hid_interface():
    """Find and return the raw HID interface of the keyboard."""
    device_interfaces = hid.enumerate(vendor_id, product_id)
    raw_hid_interfaces = [i for i in device_interfaces if i['usage_page'] == usage_page and i['usage'] == usage]

    if len(raw_hid_interfaces) == 0:
        return None

    interface = hid.Device(path=raw_hid_interfaces[0]['path'])

    print(f"Manufacturer: {interface.manufacturer}")
    print(f"Product: {interface.product}")

    return interface

def get_system_data():
    """Fetch system information (CPU load and temperature)."""
    # Get CPU load
    cpu_load = int(psutil.cpu_percent(interval=1))

    # Get CPU temperature
    try:
        w = wmi.WMI(namespace="root\\OpenHardwareMonitor")
        sensors = w.Sensor()
    
        # Filter for temperature sensors
        temps = [(sensor.Value, sensor.Name) for sensor in sensors if sensor.SensorType  == "Temperature"]
        print(temps)
        cpu_temp = int([(sensor.Value, sensor.Name) for sensor in sensors if sensor.Name == "CPU Package"][1][0])
        gpu_temp = int([(sensor.Value, sensor.Name) for sensor in sensors if sensor.Name == "GPU Core"][1][0])
       
        #print(temps)
    except Exception as e:
        print("Failed to fetch temperatures:", e)
        cpu_temp = 0  # Default to 0 if temperature fetching fails
        gpu_temp = 0

    return cpu_load, cpu_temp, gpu_temp

def send_raw_report(data):
    """Send a raw HID report to the keyboard."""
    interface = get_raw_hid_interface()

    if interface is None:
        print("No device found")
        return
        

    request_data = [0x00] * (report_length)  # First byte is Report ID
    request_data[1:len(data) + 1] = data
    request_report = bytes(request_data)
    print(len(request_report))
    print("Request:")
    print(request_report)

    try:
        interface.write(request_report)

        response_report = interface.read(report_length, timeout=1000)

        print("Response:")
        print(response_report)
    finally:
        interface.close()
# -------------------- END OF YOUR ORIGINAL CODE --------------------

# New additions for background operation
def run_as_service(interval=5):
    """Run the monitoring in background with given interval"""
    while True:
        try:
            # Your original main logic
            cpu_load, cpu_temp, gpu_temp = get_system_data()
            print(f"CPU Load: {cpu_load}% | CPU Temp: {cpu_temp}°C | GPU Temp: {gpu_temp}°C")
            
            formatted_data = f"{cpu_load}%|{cpu_temp}C|{gpu_temp}C"
            if len(formatted_data) > report_length:
                formatted_data = formatted_data[:report_length]

            send_raw_report([ord(c) for c in formatted_data])
        except Exception as e:
            print(f"Error in service loop: {e}")
        
        time.sleep(interval)

def create_startup_shortcut():
    """Create startup shortcut for current script"""
    startup_path = os.path.join(
        os.getenv('APPDATA'),
        'Microsoft\\Windows\\Start Menu\\Programs\\Startup',
        'KeyboardMonitor.lnk'
    )
    
    if not os.path.exists(startup_path):
        import winshell
        from win32com.client import Dispatch
        
        shell = Dispatch('WScript.Shell')
        shortcut = shell.CreateShortCut(startup_path)
        shortcut.TargetPath = sys.executable
        shortcut.Arguments = f'"{os.path.abspath(__file__)}"'
        shortcut.WorkingDirectory = os.getcwd()
        shortcut.save()

if __name__ == '__main__':
    # Install to startup if requested
    if '--install' in sys.argv:
        create_startup_shortcut()
        print("Installed to startup. Now running in background...")
    
    # Start background service
    try:
        run_as_service(interval=5)  # Update every 5 seconds
    except KeyboardInterrupt:
        print("Service stopped")