use esp_idf_svc::hal::{gpio::*, prelude::Peripherals};

const FINGER_MIN: i16 = 0;
const FINGER_MAX: i16 = 4095;

const FINGER_POWER_PINS: [i8; 5] = [0, 1, 2, 3, 4];
const FINGER_SIGNAL_PINS: [i8; 5] = [5, 6, 7, 8, 9];

static mut finger_values: [i16; 5] = [0, 0, 0, 0, 0];

struct FingerIOPair<'a> {
    input: PinDriver<'a, AnyIOPin, Input>,
    output: PinDriver<'a, AnyIOPin, Output>
}
impl<'a> FingerIOPair<'_> {
    fn new(i: PinDriver<'_, AnyIOPin, Input>, o: PinDriver<'_, AnyIOPin, Output>) -> FingerIOPair<'_>
    {
        FingerIOPair {input: i, output, o};
    }
}


fn main() {
    // It is necessary to call this function once. Otherwise some patches to the runtime
    // implemented by esp-idf-sys might not link properly. See https://github.com/esp-rs/esp-idf-template/issues/71
    esp_idf_svc::sys::link_patches();
    

    // Bind the log crate to the ESP Logging facilities
    esp_idf_svc::log::EspLogger::initialize_default();

    log::info!("Hello, world!");

    let peripherals = Peripherals::take().unwrap();
    let thumb = 

    let mut thumb_read = PinDriver::input(peripherals.pins.gpio0).unwrap();
    let mut index_read = PinDriver::input(peripherals.pins.gpio1).unwrap();
    let mut middle_read = PinDriver::input(peripherals.pins.gpio2).unwrap();
    let mut ring_read = PinDriver::input(peripherals.pins.gpio3).unwrap();
    let mut pinky_read = PinDriver::input(peripherals.pins.gpio4).unwrap();

    
}


fn cycleFingers(thumb: PinDriver<'_, AnyIOPin, Input>, index: PinDriver<'_, AnyIOPin, Input>, middle: PinDriver<'_, AnyIOPin, Input>, ring: PinDriver<'_, AnyIOPin, Input>, pinky: PinDriver<'_, AnyIOPin, Input>)
{
    let mut pins: [Option<PinDriver<'_, AnyIOPin, Input>>; 5] =
    [
        Some(thumb),
        Some(index),
        Some(middle),
        Some(ring),
        Some(pinky)
    ];
    
    for findex in 0..5
    {
        // Borrow input pin from the array
        let current_input_pin = pins[findex].take().expect("Pin shouldn't be none");


        // Convert to output
        let current_output_pin = current_input_pin.into_output().unwrap();

        let next_input_pin = current_output_pin.into_input().unwrap();

        pins[findex] = Some(next_input_pin);
    }
}