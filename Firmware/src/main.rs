use std::sync::Mutex;

use esp_idf_svc::hal::{gpio::*, prelude::Peripherals};
use once_cell::sync::Lazy;

const FINGER_MIN: i16 = 0;
const FINGER_MAX: i16 = 4095;

static mut FINGER_VALUES: [i16; 5] = [0, 0, 0, 0, 0];

const FINGER_POWER_PINS: Mutex<Lazy<[PinDriver<'_, AnyOutputPin, Output>; 5]>> = Mutex::new(once_cell::sync::Lazy::new(|| {
    [
        PinDriver::output(unsafe {AnyOutputPin::new(0)}).unwrap(),
        PinDriver::output(unsafe {AnyOutputPin::new(0)}).unwrap(),
        PinDriver::output(unsafe {AnyOutputPin::new(0)}).unwrap(),
        PinDriver::output(unsafe {AnyOutputPin::new(0)}).unwrap(),
        PinDriver::output(unsafe {AnyOutputPin::new(0)}).unwrap(),
    ]
}));

static FINGER_SIGNAL_PINS: Mutex<Lazy<[PinDriver<'_, AnyInputPin, Input>; 5]>> = Mutex::new(once_cell::sync::Lazy::new(|| {
    [
        PinDriver::input(unsafe {AnyInputPin::new(10)}).unwrap(),
        PinDriver::input(unsafe {AnyInputPin::new(11)}).unwrap(),
        PinDriver::input(unsafe {AnyInputPin::new(12)}).unwrap(),
        PinDriver::input(unsafe {AnyInputPin::new(13)}).unwrap(),
        PinDriver::input(unsafe {AnyInputPin::new(14)}).unwrap(),
    ]
}));

struct FingerIOPair<'a> {
    input: PinDriver<'a, AnyIOPin, Input>,
    output: PinDriver<'a, AnyIOPin, Output>
}
impl<'a> FingerIOPair<'_> {
    fn new(i: PinDriver<'_, AnyIOPin, Input>, o: PinDriver<'_, AnyIOPin, Output>) -> FingerIOPair<'_>
    {
        FingerIOPair {input: i, output: o};
    }
}


fn main() {
    
    esp_idf_svc::sys::link_patches();
    

    // Bind the log crate to the ESP Logging facilities
    esp_idf_svc::log::EspLogger::initialize_default();

    log::info!("Hello, world!");

    let peripherals = Peripherals::take().unwrap();
    
    loop {
        
        cycleFingers();


    }

}


fn cycleFingers()
{
    
    
    for findex in 0..5
    {
        // Borrow input pin from the array
        let current_power = FINGER_POWER_PINS.lock().unwrap();
        let current_input = FINGER_SIGNAL_PINS.lock().unwrap();
        
        
        current_power.
        

        drop(current_power);
        drop(current_input);
    }
}