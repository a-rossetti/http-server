#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <arpa/inet.h>

const int PORT = 8080;          // The port number the server will listen on.
const int BUFFER_SIZE = 1024;   // The size of the buffer to store incoming requests.

// Function to generate a basic HTTP response.
std::string generateHttpResponse(const std::string& content, const std::string& contentType = "text/html") {
    std::ostringstream oss;
    oss << "HTTP/1.1 200 OK\r\n"
        << "Content-Type: " << contentType << "\r\n"
        << "Content-Length: " << content.length() << "\r\n"
        << "Connection: close\r\n"
        << "\r\n"
        << content;
    return oss.str();
}

// Function to read file content
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nFile Not Found";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Function to check if a string ends with a given suffix
bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.size() < suffix.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

// Function to determine the content type based on its file extension
std::string getContentType(const std::string& filePath) {
    if (endsWith(filePath, ".html")) return "text/html";
    if (endsWith(filePath, ".css")) return "text/css";
    if (endsWith(filePath, ".js")) return "application/javascript";
    if (endsWith(filePath, ".jpg") || endsWith(filePath, ".jpeg")) return "image/jpeg";
    if (endsWith(filePath, ".png")) return "image.png";
    return "text/plain";  // Default content type
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Allow port to be reused quickly after the server terminates.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Bind socket to a specific IP address and port number.
    address.sin_family = AF_INET;           // Specifies the use of IPv4
    address.sin_addr.s_addr = INADDR_ANY;   // Server accepts connections on any available network interfaces
    address.sin_port = htons(PORT);         // Sets port number (to 8080)

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Set up the server socket to listen for incoming connections.
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // Open the default web browser to localhost:8080
    std::string command = "xdg-open http://localhost:8080";  // For Linux
    #ifdef _WIN32
        command = "start http://localhost:8080";  // For Windows
    #elif __APPLE__
        command = "open http://localhost:8080";  // For macOS
    #endif
    system(command.c_str());

    // Accept connections and handle client requests.
    while (true) {
        // Accepting a connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Read the request
        read(new_socket, buffer, BUFFER_SIZE);
        std::cout << "Received request:\n" << buffer << std::endl;

        // Parse the request to get the file path
        std::string request(buffer);
        std::size_t pos = request.find("GET /");
        std::string filePath = "index.html";  // Default file
        if (pos != std::string::npos) {
            std::size_t start = pos + 5;  // Length of "GET /"
            std::size_t end = request.find(" ", start);
            filePath = request.substr(start, end - start);
            if (filePath.empty() || filePath == "/") {
                filePath = "index.html";
            }
        }

        // Read the file content
        std::string fileContent = readFile(filePath);

        // Determine content type
        std::string contentType = getContentType(filePath);

        // Generate response
        std::string response = generateHttpResponse(fileContent, contentType);

        // Send the response to the client
        send(new_socket, response.c_str(), response.size(), 0);
        std::cout << "Response sent\n";

        // Close the socket
        close(new_socket);

    }

    return 0;
}
