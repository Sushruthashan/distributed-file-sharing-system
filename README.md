# Implementing a Distributed File Sharing System with Poco C++ Libraries

A secure and scalable distributed file sharing system built using **C++**, **Poco C++ Libraries**, and **YugabyteDB**. The system allows users to upload, download, and share files via secure shareable links.

## Features

- Secure file uploads and downloads
- Shareable links for file access using unique tokens
- Files stored in a distributed database (YugabyteDB)
- Built with CMake for cross-platform compatibility
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
<pre> 
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
</pre>
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
curl -OJL http://localhost:9090/download/<share_token>


## Key Design Decisions
1. I was facing difficulties starting the YSQL shell, hence decided to create manipulate the database tables within the C++ files itself
2. Used PostgreSQL along with Poco, where Poco handling the web server and HTTP request/response and PostgreSQL handling the storage of files, helping in writing modular and maintainable C++ codes.
   The combination also enabled fast-file sharing system with added security as Poco supports HTTP authentication easily and PostgreSQL offers authentication, encryption(TLS) and access control.


## Challenges faced
1. I was unfamiliar with Poco and PostgreSQL libraries along with Yugabyte DB, but eventually learnt them and implemented them for the project.
2. Faced difficulties starting the YSQL shell, managed it through C++ manipulation of the database.

   
## Testing

Tested with:

* Text files up to 100MB
* Empty and duplicate file uploads
* Invalid or expired tokens

## Performance

* Instant response from distributed storage (YugabyteDB)
* Files stored directly in DB, reducing file I/O overhead
