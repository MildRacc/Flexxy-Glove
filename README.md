# Flexxy Glove

**Flexxy Glove** is a hardware + software project that demonstrates finger motion tracking using a glove equipped with flexible TPU joints and flex sensors. Unlike other VR-focused glove designs (such as the LucasTech VR glove, which relies on rigid plastic, strings, and potentiometers), Flexxy Glove prioritizes durability, comfort, and simplicity in construction.

This project is **not limited to VR**. Instead, it comes with a **visualizer program** that displays real-time glove input via a self-hosted website. A lightweight C++ web server runs locally, handling both the glove interface and the visualization simultaneously.

## Features
- Flexible TPU joints for a more natural fit and comfort.  
- Flex sensors for tracking finger motion.  
- Real-time visualization of finger data.  
- Self-hosted C++ web server that:  
  - Interfaces with the glove hardware.  
  - Hosts a local website for live visualization.  

## Hardware
- Glove base with flexible TPU joints.  
- Flex sensors for each finger.  
- Microcontroller (compatible board of your choice).  

## Software
- C++ server backend to:  
  - Read sensor data from the glove.  
  - Serve a visualization website.  
- Visualization front-end to display finger positions.  
