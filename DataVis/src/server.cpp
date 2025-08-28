#include </home/sashad/Arduino/HandController/DataVis/libs/httplib.h>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <map>

// Helper function to determine MIME type based on file extension
std::string get_mime_type(const std::string& file_path) {
    std::map<std::string, std::string> mime_types = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        // Add more MIME types as needed
    };

    // Get file extension
    std::string ext = file_path.substr(file_path.find_last_of('.'));
    if (mime_types.find(ext) != mime_types.end()) {
        return mime_types[ext];
    }
    
    return "application/octet-stream";  // Default MIME type for unknown file types
}

void serve_file(const std::string& file_path, httplib::Response& res) {
    // Open the requested file
    std::ifstream file(file_path, std::ios::binary);
    
    if (!file.is_open()) {
        res.status = 404;
        res.set_content("File not found", "text/plain");
        return;
    }

    // Read the file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Get the correct MIME type for the file
    std::string mime_type = get_mime_type(file_path);

    // Set the response with the correct content type
    res.set_content(buffer.str(), mime_type);
}

void start_server() {
    // Open the HTML file
    std::ifstream file("./../site/index.html");
        
    // Check if the file opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open index.html\n";
        return;
    }

    // Read the file into a string
    std::string html_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Check if the content was successfully read
    if (html_content.empty()) {
        std::cerr << "Error: HTML file is empty or not read properly\n";
        return;
    }

    // Create the server object
    httplib::Server server;

    // Serve the HTML content
    server.Get("/", [&html_content](const httplib::Request&, httplib::Response& res) {
        res.set_content(html_content, "text/html");
    });

    // Serve the main.js file with the correct MIME type
    server.Get("/dist/bundle.js", [](const httplib::Request&, httplib::Response& res) {
        serve_file("./../site/dist/bundle.js", res);
    });

    // Serve the styles.css file with the correct MIME type
    server.Get("/styles.css", [](const httplib::Request&, httplib::Response& res) {
        serve_file("./../site/styles.css", res);
    });

    // Serve the Three.js modules with the correct MIME type
    server.Get("/node_modules/three/build/three.module.js", [](const httplib::Request&, httplib::Response& res) {
        serve_file("./../site/node_modules/three/build/three.module.js", res);
    });

    server.Get("/node_modules/three/build/three.core.js", [](const httplib::Request&, httplib::Response& res) {
        serve_file("./../site/node_modules/three/build/three.core.js", res);
    });

    server.Get("/node_modules/three/examples/jsm/controls/OrbitControls.js", [](const httplib::Request&, httplib::Response& res) {
        serve_file("./../site/node_modules/three/examples/jsm/controls/OrbitControls.js", res);
    });

    // Add other Three.js module files if needed
    // Example:
    // server.Get("/node_modules/three/build/three.js", [](const httplib::Request&, httplib::Response& res) {
    //     serve_file("./../site/node_modules/three/build/three.js", res);
    // });

    // Start the server
    std::cout << "Server running at http://localhost:8080\n";
    server.listen("localhost", 8080);
}