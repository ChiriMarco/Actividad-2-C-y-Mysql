#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;

void crearSucursal(sql::Connection* con) {
    string nombre, ubicacion, fecha;
    int trabajadores;
    bool activo;

    cout << "Nombre: ";
    getline(cin >> ws, nombre);
    cout << "Ubicación: ";
    getline(cin >> ws, ubicacion);
    cout << "Cantidad de trabajadores: ";
    cin >> trabajadores;
    cout << "Fecha de inauguración (YYYY-MM-DD): ";
    cin >> fecha;
    cout << "¿Activo? (1 = Sí, 0 = No): ";
    cin >> activo;

    sql::PreparedStatement* pstmt = con->prepareStatement(
        "INSERT INTO sucursales (nombre, ubicacion, cantidad_trabajadores, fecha_inauguracion, activo) VALUES (?, ?, ?, ?, ?)");
    pstmt->setString(1, nombre);
    pstmt->setString(2, ubicacion);
    pstmt->setInt(3, trabajadores);
    pstmt->setString(4, fecha);
    pstmt->setBoolean(5, activo);
    pstmt->execute();

    cout << "Sucursal creada con éxito.\n";
    delete pstmt;
}

void mostrarSucursales(sql::Connection* con) {
    sql::Statement* stmt = con->createStatement();
    sql::ResultSet* res = stmt->executeQuery("SELECT * FROM sucursales");

    while (res->next()) {
        cout << "\nID: " << res->getInt("id_sucursal")
             << "\nNombre: " << res->getString("nombre")
             << "\nUbicación: " << res->getString("ubicacion")
             << "\nTrabajadores: " << res->getInt("cantidad_trabajadores")
             << "\nFecha Inauguración: " << res->getString("fecha_inauguracion")
             << "\nActivo: " << (res->getBoolean("activo") ? "Sí" : "No") << "\n";
    }

    delete res;
    delete stmt;
}

void actualizarSucursal(sql::Connection* con) {
    int id;
    cout << "ID de la sucursal a actualizar: ";
    cin >> id;

    string nombre, ubicacion, fecha;
    int trabajadores;
    bool activo;

    cout << "Nuevo nombre: ";
    getline(cin >> ws, nombre);
    cout << "Nueva ubicación: ";
    getline(cin >> ws, ubicacion);
    cout << "Cantidad de trabajadores: ";
    cin >> trabajadores;
    cout << "Fecha de inauguración (YYYY-MM-DD): ";
    cin >> fecha;
    cout << "¿Activo? (1 = Sí, 0 = No): ";
    cin >> activo;

    sql::PreparedStatement* pstmt = con->prepareStatement(
        "UPDATE sucursales SET nombre=?, ubicacion=?, cantidad_trabajadores=?, fecha_inauguracion=?, activo=? WHERE id_sucursal=?");
    pstmt->setString(1, nombre);
    pstmt->setString(2, ubicacion);
    pstmt->setInt(3, trabajadores);
    pstmt->setString(4, fecha);
    pstmt->setBoolean(5, activo);
    pstmt->setInt(6, id);
    pstmt->execute();

    cout << "Sucursal actualizada.\n";
    delete pstmt;
}

void eliminarSucursal(sql::Connection* con) {
    int id;
    cout << "ID de la sucursal a eliminar: ";
    cin >> id;

    sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM sucursales WHERE id_sucursal=?");
    pstmt->setInt(1, id);
    pstmt->execute();

    cout << "Sucursal eliminada.\n";
    delete pstmt;
}

int main() {
    try {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "root", "tu_contraseña");
        con->setSchema("empresa");

        int opcion;
        do {
            cout << "\n--- CRUD Sucursales ---\n";
            cout << "1. Crear\n2. Mostrar\n3. Actualizar\n4. Eliminar\n0. Salir\nOpción: ";
            cin >> opcion;

            switch (opcion) {
                case 1: crearSucursal(con); break;
                case 2: mostrarSucursales(con); break;
                case 3: actualizarSucursal(con); break;
                case 4: eliminarSucursal(con); break;
            }
        } while (opcion != 0);

        delete con;
    } catch (sql::SQLException& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
