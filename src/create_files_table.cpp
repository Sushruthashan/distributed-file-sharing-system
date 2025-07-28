#include <pqxx/pqxx>
#include <iostream>

int connect() {
    try {
        pqxx::connection conn("dbname=yugabyte user=yugabyte password=yugabyte host=localhost port=5433");
        pqxx::work txn(conn);

        txn.exec(R"(
            CREATE TABLE IF NOT EXISTS files (
                id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
                username TEXT,
                filename TEXT,
                content BYTEA,
                content_type TEXT,
                size INTEGER,
                uploaded_at TIMESTAMPTZ DEFAULT now()
            );
        )");

        txn.commit();
        std::cout << "Files table created successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
