import csv
from datetime import datetime, timezone, timedelta
import matplotlib.pyplot as plt
import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QFileDialog
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


# Function to parse NMEA log
def parse_nmea_log(file_path):
    data = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    current_epoch = {
        'time': None,
        'hdop': None,
        'vdop': None,
        'satellites': None,  # Satellite Count (used in fix)
        'total_satellites': 0,  # Total satellites in view
        'satellite_counts': {'GPS': 0, 'GLONASS': 0, 'Galileo': 0, 'Beidou': 0}
    }

    for line in lines:
        line = line.strip()
        if line.startswith('$GNGGA') or line.startswith('$GPGGA'):
            fields = line.split(',')
            if len(fields) > 7 and fields[6] != '':
                current_epoch['time'] = convert_to_local_time(fields[1])
                current_epoch['satellites'] = int(fields[7])
                current_epoch['hdop'] = float(fields[8]) if fields[8] else None

        elif line.startswith('$GPGSV') or line.startswith('$GLGSV') or line.startswith('$GBGSV') or line.startswith('$GAGSV'):
            fields = line.split(',')
            if len(fields) > 3:
                total_satellites_str = fields[3]  # Extract total satellites field
                total_satellites_str = total_satellites_str.split('*')[0]  # Remove checksum part
                try:
                    total_satellites = int(total_satellites_str)
                    current_epoch['total_satellites'] = max(current_epoch['total_satellites'], total_satellites)
                except ValueError:
                    print(f"Skipping invalid total satellites value: {total_satellites_str}")

                # Determine GNSS system and count satellites
                if line.startswith('$GPGSV'):
                    current_epoch['satellite_counts']['GPS'] += (len(fields) - 4) // 4
                elif line.startswith('$GLGSV'):
                    current_epoch['satellite_counts']['GLONASS'] += (len(fields) - 4) // 4
                elif line.startswith('$GBGSV'):
                    current_epoch['satellite_counts']['Beidou'] += (len(fields) - 4) // 4
                elif line.startswith('$GAGSV'):
                    current_epoch['satellite_counts']['Galileo'] += (len(fields) - 4) // 4

        elif line.startswith('$GNGSA') or line.startswith('$GPGSA'):
            fields = line.split(',')
            if len(fields) > 17:
                if fields[17]:
                    vdop_value = fields[17].split('*')[0]  # Remove checksum part
                    current_epoch['vdop'] = float(vdop_value) if vdop_value else None


        # Append and reset current epoch if time changes or new data begins
        if current_epoch['time'] and line.startswith('$GN'):
            data.append(current_epoch)
            current_epoch = {
                'time': None,
                'hdop': None,
                'vdop': None,
                'satellites': None,
                'total_satellites': 0,
                'satellite_counts': {'GPS': 0, 'GLONASS': 0, 'Galileo': 0, 'Beidou': 0}
            }
    
    return data


# Convert UTC time to local CST time
def convert_to_local_time(utc_time):
    if not utc_time or len(utc_time) < 6:
        return None
    hour, minute, second = int(utc_time[:2]), int(utc_time[2:4]), float(utc_time[4:])
    utc_dt = datetime.now(timezone.utc).replace(hour=hour, minute=minute, second=int(second), microsecond=int((second % 1) * 1e6))
    cst_dt = utc_dt.astimezone(timezone(timedelta(hours=-6)))  # CST (UTC-6)
    return cst_dt.strftime('%Y-%m-%d %H:%M:%S')

# Write parsed data to a CSV
def write_to_csv(data, output_file):
    fieldnames = ['time', 'hdop', 'vdop', 'satellites', 'total_satellites', 'GPS', 'GLONASS', 'Galileo', 'Beidou']
    with open(output_file, 'w', newline='') as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for item in data:
            row = {key: item.get(key, None) for key in fieldnames}
            if item.get('satellite_counts'):
                row.update(item['satellite_counts'])
            writer.writerow(row)

# PyQt5 Window Class to Display Matplotlib Plots
class PlotWindow(QMainWindow):
    def __init__(self, data):
        super().__init__()
        self.data = data
        self.initUI()
    
    def initUI(self):
        self.setWindowTitle("NMEA Data Visualization")
        self.setGeometry(100, 100, 1200, 800)
        
        # Main widget and layout
        main_widget = QWidget(self)
        self.setCentralWidget(main_widget)
        layout = QVBoxLayout(main_widget)

        # Add Satellite Count Plot
        satellite_canvas = FigureCanvas(self.create_satellite_plot())
        layout.addWidget(satellite_canvas)

        # Add GNSS System Plot
        gnss_canvas = FigureCanvas(self.create_gnss_plot())
        layout.addWidget(gnss_canvas)

    def create_satellite_plot(self):
        # Create the figure for satellite counts
        fig = Figure(figsize=(12, 6))
        ax = fig.add_subplot(111)

        times = [item['time'] for item in self.data if item['time']]
        satellites = [item['satellites'] for item in self.data]
        total_satellites = [item['total_satellites'] for item in self.data]

        ax.plot(times, satellites, label='Satellites Used (Fix)', marker='o')
        ax.plot(times, total_satellites, label='Total Satellites in View', marker='x')

        ax.set_xlabel('Time (CST)')
        ax.set_ylabel('Satellite Count')
        ax.set_title('Satellite Counts Over Time')
        ax.legend()
        ax.grid()

        return fig

    def create_gnss_plot(self):
        # Create the figure for GNSS system counts
        fig = Figure(figsize=(12, 6))
        ax = fig.add_subplot(111)

        times = [item['time'] for item in self.data if item['time']]
        gps_counts = [item['satellite_counts']['GPS'] for item in self.data]
        glonass_counts = [item['satellite_counts']['GLONASS'] for item in self.data]
        galileo_counts = [item['satellite_counts']['Galileo'] for item in self.data]
        beidou_counts = [item['satellite_counts']['Beidou'] for item in self.data]

        ax.plot(times, gps_counts, label='GPS', marker='o')
        ax.plot(times, glonass_counts, label='GLONASS', marker='x')
        ax.plot(times, galileo_counts, label='Galileo', marker='s')
        ax.plot(times, beidou_counts, label='Beidou', marker='d')

        ax.set_xlabel('Time (CST)')
        ax.set_ylabel('Satellite Count by System')
        ax.set_title('Satellite System Counts Over Time')
        ax.legend()
        ax.grid()

        return fig

# Function to launch PyQt5 PlotWindow
def generate_plots(data):
    app = QApplication(sys.argv)
    window = PlotWindow(data)
    window.show()
    sys.exit(app.exec_())

# Main script
def get_input_file():
    file_path, _ = QFileDialog.getOpenFileName(None, "Select NMEA Log File", "", "Text Files (*.txt);;All Files (*)")
    return file_path

if __name__ == "__main__":
    # Ensure QApplication is created first
    app = QApplication(sys.argv)

    # Get NMEA log file from user input
    input_file = get_input_file()
    
    if not input_file:
        print("No file selected, exiting.")
        sys.exit(1)

    output_csv = 'parsed_nmea_data.csv'

    data = parse_nmea_log(input_file)
    write_to_csv(data, output_csv)
    generate_plots(data)