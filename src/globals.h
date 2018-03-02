#if !defined(globals_h_)
#define globals_h_

extern pid_data_t sensorPIDData[3];

// Pins
extern const uint32_t notifyPin1;
extern const uint32_t notifyPin2;
extern const uint32_t heaterPin;
extern const uint32_t temp1Pin;
extern const uint32_t temp2Pin;
extern const uint32_t pumpPinOut;
extern const uint32_t temppumpGnd_pin;
extern const uint32_t idac0Pin;
extern const uint32_t idacPin;
extern const uint32_t pumpPinIn;

// Espresso Machine modes
enum {
    mode_pump_off,   // Heating to espresso temp, pump off
    mode_pump_on,    // Pulling espresso shot
    mode_steaming    // Heating at espresso temp
};

#define HEATER_OFF 0
#define DIVIDE_INT(num, dnm) ((num + dnm / 2) / dnm)

typedef struct {
    uint8_t set;
    uint8_t water;
    uint8_t boiler;
    uint8_t power;
    uint8_t state;
} log_data_t;


#endif // !defined(globals_h_)
