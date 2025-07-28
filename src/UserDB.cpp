#include <iostream>
#include <pqxx/pqxx>

int connect1() {
    try {
        // Connect to YugabyteDB (PostgreSQL-compatible)
        pqxx::connection conn("host=localhost port=5433 dbname=yugabyte user=yugabyte password=yugabyte");

        // Start transaction
        pqxx::work txn(conn);

        // SQL to create users table
        std::string query = R"(
            CREATE TABLE IF NOT EXISTS users (
                id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
                username TEXT UNIQUE NOT NULL,
                password_hash TEXT NOT NULL
            )
        )";

        // Execute
        txn.exec(query);
        txn.commit();

        std::cout << "Table created successfully." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
