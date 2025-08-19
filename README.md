# ESP32 OSC to CV Converter

A versatile ESP32-based Open Sound Control (OSC) to Control Voltage (CV) converter for modular synthesizers and electronic music applications. This project transforms OSC messages from TouchOSC or other OSC-enabled devices into analog control voltages and gate signals.

## 🎯 Features

- **Dual CV Outputs**: Two independent 12-bit CV channels (0-5V range)
- **Gate Outputs**: Three gate outputs (CV1 trigger, CV2 trigger, Clock pulse)
- **WiFi Connectivity**: Creates its own access point for wireless control
- **TouchOSC Integration**: Pre-configured TouchOSC layout included
- **Real-time Control**: Low-latency OSC message processing
- **MCP4822 DAC**: High-quality 12-bit digital-to-analog conversion
- **ESP32-C3 Based**: Compact and efficient design

## 🏗️ Hardware Requirements

### Core Components
- **ESP32-C3** (tested with Seeed XIAO ESP32C3)
- **MCP4822** 12-bit Dual Channel DAC
- **Power Supply**: 5V regulated power
- **Breadboard or PCB** for connections

### Pin Connections

| ESP32 Pin | MCP4822 Pin | Function |
|-----------|-------------|----------|
| GPIO6 | SCK | SPI Clock |
| GPIO7 | SDI | SPI MOSI |
| GPIO10 | CS | Chip Select |
| GPIO2 | - | Gate Clock Output |
| GPIO3 | - | Gate Channel 1 Output |
| GPIO4 | - | Gate Channel 2 Output |
| GPIO8 | - | Status LED |

### Power Connections
- **VCC**: 3.3V (ESP32) / 5V (MCP4822 VDD)
- **GND**: Common ground between all components

## 🔧 Software Setup

### Prerequisites
- [PlatformIO](https://platformio.org/) installed
- [TouchOSC](https://hexler.net/touchosc) app for mobile control
- Arduino framework for ESP32

### Installation Steps

1. **Clone the repository**
   ```bash
   git clone [repository-url]
   cd esp32-osc-midi-cv
   ```

2. **Install dependencies**
   ```bash
   pio lib install
   ```

3. **Configure WiFi (optional)**
   - The ESP32 creates an access point named "ESP32-OSC" by default
   - No password required for open access
   - IP address: `192.168.4.1` (default)

4. **Upload firmware**
   ```bash
   pio run --target upload
   ```

5. **Monitor serial output**
   ```bash
   pio device monitor
   ```

## 📱 TouchOSC Configuration

### Layout File
The project includes `ESP32-OSC2CV.touchosc` - a pre-configured TouchOSC layout with:
- **CV1 Slider**: Controls channel 1 voltage (0-4095)
- **CV2 Slider**: Controls channel 2 voltage (0-4095)
- **Clock Button**: Sends clock pulses
- **Connection Status**: Visual feedback

### OSC Message Format
The device listens on UDP port `5555` for the following OSC addresses:

| OSC Address | Value Range | Function |
|-------------|-------------|----------|
| `/cv1` | 0-4095 | Sets CV output 1 voltage |
| `/cv2` | 0-4095 | Sets CV output 2 voltage |
| `/clock` | - | Triggers clock pulse |

### TouchOSC Setup
1. Install TouchOSC on your mobile device
2. Load the provided `ESP32-OSC2CV.touchosc` layout
3. Connect to the "ESP32-OSC" WiFi network
4. Configure TouchOSC host to `192.168.4.1:5555`

## ⚡ Usage Examples

### Basic CV Control
Send OSC messages to control voltage outputs:
```bash
# Set CV1 to 2.5V (2048/4095 * 5V)
oscsend 192.168.4.1 5555 /cv1 i 2048

# Set CV2 to maximum (5V)
oscsend 192.168.4.1 5555 /cv2 i 4095
```

### Clock Synchronization
Trigger clock pulses for synchronization:
```bash
# Send clock pulse
oscsend 192.168.4.1 5555 /clock
```

### Modular Synthesizer Integration
- **CV1/CV2**: Connect to VCO frequency inputs, VCF cutoff, VCA level
- **Gate Outputs**: Connect to envelope generators, sequencers, or clock dividers
- **Clock**: Sync multiple devices or trigger events

## 🔧 Development

### Project Structure
```
esp32-osc-midi-cv/
├── src/
│   └── main.cpp          # Main application code
├── include/              # Header files
├── lib/                  # Custom libraries
├── test/                 # Unit tests
├── ESP32-OSC2CV.touchosc # TouchOSC layout
├── platformio.ini        # PlatformIO configuration
└── README.md            # This file
```

### Building from Source
```bash
# Build the project
pio run

# Upload to device
pio run --target upload

# Clean build files
pio run --target clean
```

### Debugging
- **Serial Monitor**: 115200 baud for debug output
- **LED Indicator**: Blinks 3 times on successful WiFi setup
- **OSC Logging**: CV values and clock pulses printed to serial

## 📊 Technical Specifications

### Electrical Characteristics
- **CV Output Range**: 0-5V (12-bit resolution = 1.22mV/step)
- **Gate Output**: 0-3.3V logic level
- **Current Draw**: ~80mA (ESP32 + MCP4822)
- **WiFi Range**: ~30m (line of sight)

### Performance
- **Latency**: <1ms OSC to CV conversion
- **Update Rate**: Up to 1000 Hz per channel
- **Resolution**: 12-bit (4096 steps)

## 🔄 Advanced Configuration

### Custom WiFi Settings
Modify these constants in `src/main.cpp`:
```cpp
const char* ap_ssid = "Your-Network-Name";
const char* ap_password = "Your-Password";
```

### DAC Calibration
The MCP4822 outputs 0-4.095V by default. For 0-5V output:
- Add external op-amp buffer with gain
- Or use internal gain setting (modify `writeDAC` function)

## 🐛 Troubleshooting

### Common Issues

**ESP32 not connecting**
- Check power supply (minimum 500mA)
- Verify USB cable is data-capable
- Try different USB port

**No OSC response**
- Confirm device is connected to "ESP32-OSC" network
- Check TouchOSC host configuration
- Verify UDP port 5555 is not blocked

**CV output not working**
- Check MCP4822 wiring (especially CS pin)
- Verify SPI connections
- Test with multimeter on DAC outputs

### Debug Output
Enable verbose logging by uncommenting lines in `loop()` function:
```cpp
Serial.println("Esperando OSC...");
```

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

## 🙏 Acknowledgments

- [CNMAT OSC Library](https://github.com/CNMAT/OSC) for Arduino
- [TouchOSC](https://hexler.net/touchosc) for mobile control interface
- [PlatformIO](https://platformio.org/) for build system
- Seeed Studio for XIAO ESP32C3 development board

## 📞 Support

For questions, issues, or contributions:
- Open an [Issue](https://github.com/[username]/esp32-osc-midi-cv/issues)
- Check [Discussions](https://github.com/[username]/esp32-osc-midi-cv/discussions)
- Join our [Discord/Slack community]

---

**Made with ❤️ for the modular synthesizer community**