# Distributed File Sharing System

A secure and scalable distributed file sharing system built using **C++**, **Poco C++ Libraries**, and **YugabyteDB**. The system allows users to upload, download, and share files via secure shareable links. Designed for performance, portability, and extensibility.

## Features

- Secure file uploads and downloads
- Shareable links for file access using unique tokens
- Files stored in a distributed database (YugabyteDB)
- ⚙Built with CMake for cross-platform compatibility
- HTTP-based API using Poco C++ Server framework

## Tech Stack

| Layer              | Technology            |
|--------------------|------------------------|
| Language           | C++                    |
| Networking         | [Poco C++ Libraries](https://pocoproject.org/) |
| Storage            | [YugabyteDB](https://www.yugabyte.com/) (PostgreSQL-compatible) |
| PostgreSQL Client  | libpqxx                |
| Build System       | CMake                  |

## Project Structure

distributed-file-sharing/
├── CMakeLists.txt
├── main.cpp
├── handlers/
│   ├── RegisterHandler.cpp
│   ├── LoginHandler.cpp
│   ├── FileUploadHandler.cpp
│   ├── FileDownloadHandler.cpp
│   ├── NotFoundHandler.cpp
├── db/
│   └── Database.cpp
├── utils/
│   └── TokenGenerator.cpp
├── .gitignore
└── README.md

## Getting Started

### Prerequisites

- C++17 or later
- Poco C++ Libraries
- libpqxx
- YugabyteDB (running locally or via Docker)

### Installation & Build

#### 1. **Clone the Repository**
git clone https://github.com/Sushruthashan/distributed-file-sharing.git
cd distributed-file-sharing

#### 2. **Install Dependencies**
sudo apt update
sudo apt install libpoco-dev libpqxx-dev cmake g++

#### 3. **Build the Project**
mkdir build && cd build
cmake ..
make

#### 4. **Start YugabyteDB (via Docker)**
docker run -d --name yugabyte -p 5433:5433 -p 7000:7000 yugabytedb/yugabyte:latest bin/yugabyted start --daemon=false

> Ensure YugabyteDB is ready before running the app.

## API Endpoints

### Upload a File

* **Form Field:** `file`
* **Returns:** shareable token

curl -X POST -F "file=@yourfile.txt" http://localhost:8080/upload

### Download a File
curl -OJL http://localhost:8080/download/<share_token>

## Testing

Tested with:

* Large files up to 100MB
* Empty and duplicate file uploads
* Invalid or expired tokens
* Parallel upload/download requests

## Performance

* Instant response from distributed storage (YugabyteDB)
* Files stored directly in DB, reducing file I/O overhead

## Author

**Sushrutha Shanbhogue**
[GitHub](https://github.com/Sushruthashan) 
