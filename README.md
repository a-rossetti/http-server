# Simple HTTP Server in C++

This is a simple HTTP server written in C++. It serves static files such as HTML, CSS, JavaScript, and images. The server listens on port 8080 and can be accessed via `http://localhost:8080`.

## Features

- Serves static files (HTML, CSS, JS, images)
- Automatically opens the default web browser to the server's URL on start
- Handles different content types based on file extensions

## Getting Started

### Prerequisites

- C++ compiler (e.g., `g++`)
- Git (for version control)

### Running the Server

1. Clone the repository:
    ```sh
    git clone https://github.com/a-rossetti/http-server.git
    cd http-server
    ```

2. Compile the server:
    ```sh
    g++ -o http_server http_server.cpp
    ```

3. Create and place your HTML, CSS, JS, and image files in the same directory as the server executable.

4. Run the server:
    ```sh
    ./http_server
    ```

The server will start and automatically open your default web browser to `http://localhost:8080`.

### License

This project is licensed under the MIT License.
