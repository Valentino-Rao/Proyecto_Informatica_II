#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iomanip>
#include <set>

class SerialPort {
    public:
        SerialPort(const std::string& port, int baud_rate) {
            serial_port = open(port.c_str(), O_RDWR);

            if (serial_port < 0) {
                std::cerr << "Error al abrir el puerto serial" << std::endl;
                exit(1);
            }

            struct termios tty;
            if(tcgetattr(serial_port, &tty) != 0) {
                std::cerr << "Error al obtener los atributos del puerto serial" << std::endl;
                exit(1);
            }

            cfsetispeed(&tty, baud_rate);
            cfsetospeed(&tty, baud_rate);

            tty.c_cflag &= ~PARENB;
            tty.c_cflag &= ~CSTOPB;
            tty.c_cflag &= ~CSIZE;
            tty.c_cflag |= CS8;

            tty.c_cflag &= ~CRTSCTS;
            tty.c_cflag |= CREAD | CLOCAL;

            tty.c_lflag &= ~ICANON;
            tty.c_lflag &= ~ECHO;
            tty.c_lflag &= ~ECHOE;
            tty.c_lflag &= ~ECHONL;
            tty.c_lflag &= ~ISIG;
            tty.c_iflag &= ~(IXON | IXOFF | IXANY);
            tty.c_iflag &= ~(ICRNL | INLCR);
            tty.c_oflag &= ~OPOST;
            tty.c_oflag &= ~ONLCR;

            tty.c_cc[VTIME] = 10;
            tty.c_cc[VMIN] = 0;

            if(tcsetattr(serial_port, TCSANOW, &tty) != 0) {
                std::cerr << "Error al configurar el puerto serial" << std::endl;
                exit(1);
            }
        }

        ~SerialPort() {
            close(serial_port);
        }

    std::string readData() {
        std::string result;
        char read_buf[256];
        while (true) {
            memset(&read_buf, '\0', sizeof(read_buf));
            int num_bytes = read(serial_port, &read_buf, sizeof(read_buf) - 1);

            if (num_bytes < 0) {
                std::cerr << "Error al leer del puerto serial" << std::endl;
                return "";
            }

            result.append(read_buf, num_bytes);

            if (result.find('\n') != std::string::npos) {
                break;
            }
        }

        return result;
    }
    private:
        int serial_port;
};

void processData(const std::string& data) {
    std::istringstream ss(data);
    std::string token;
    while (std::getline(ss, token, ',')) {
        std::cout << token << std::endl;
    }
}

class Persona {
    public:
        Persona(float calorias, float proteinas, float carbohidratos, float lipidos): calorias_diarias(calorias), proteinas_diarias(proteinas), carbohidratos_diarios(carbohidratos), lipidos_diarios(lipidos) {}

        float getCaloriasDiarias() const { return calorias_diarias; }
        float getProteinasDiarias() const { return proteinas_diarias; }
        float getCarbohidratosDiarios() const { return carbohidratos_diarios; }
        float getLipidosDiarios() const { return lipidos_diarios; }

    private:
        float calorias_diarias;
        float proteinas_diarias;
        float carbohidratos_diarios;
        float lipidos_diarios;
};


struct FoodData {
    std::string food;
    float weight;
};

std::vector<FoodData> readDataFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    std::vector<FoodData> foodDataList;
    std::string line;

    if (!infile.is_open()) {
        std::cerr << "Error al abrir el archivo para leer" << std::endl;
        return foodDataList;
    }

    while (std::getline(infile, line)) {
        std::istringstream ss(line);
        FoodData foodData;
        std::getline(ss, foodData.food, ',');
        ss >> foodData.weight;
        foodDataList.push_back(foodData);
    }

    infile.close();
    return foodDataList;
}

struct NutritionalInfo {
    float calorias;
    float proteinas;
    float carbohidratos;
    float lipidos;
};

NutritionalInfo calculateNutritionalInfo(const std::string& food, float weight) {
    NutritionalInfo info = {0, 0, 0, 0};
    if (food == "Pollo") {
        info.calorias = weight * 2.39; // 2.39 calorías por gramo
        info.proteinas = weight * 0.27; // 0.27 gramos de proteína por gramo
        info.carbohidratos = weight * 0.17; // 0 gramos de carbohidratos por gramo
        info.lipidos = weight * 0.14; // 0.14 gramos de lípidos por gramo
    } else if (food == "Pescado") {
        info.calorias = weight * 2.46; // 2.06 calorías por gramo
        info.proteinas = weight * 0.23; // 0.22 gramos de proteína por gramo
        info.carbohidratos = weight * 0.09; // 0 gramos de carbohidratos por gramo
        info.lipidos = weight * 0.13; // 0.12 gramos de lípidos por gramo
    } else if (food == "Pizza") {
        info.calorias = weight * 2.66; // 2.66 calorías por gramo
        info.proteinas = weight * 0.11; // 0.11 gramos de proteína por gramo
        info.carbohidratos = weight * 0.33; // 0.33 gramos de carbohidratos por gramo
        info.lipidos = weight * 0.10; // 0.10 gramos de lípidos por gramo
    } else if (food == "Cebolla"){
        info.calorias = weight * 0.40; // 0.40 calorías por gramo
        info.proteinas = weight * 0.01; // 0.01 gramos de proteína por gramo
        info.carbohidratos = weight * 0.09; // 0.09 gramos de carbohidratos por gramo
        info.lipidos = weight * 0.01; // 0.01 gramos de lípidos por gramo
    }

    return info;
}

void imprimirReceta(const std::string& comida) {
    if (comida == "Pollo") {
        std::cout << "Receta para Pollo al Horno:\n";
        std::cout << "1. Precalentar el horno a 180 grados Celsius.\n";
        std::cout << "2. Sazonar el pollo con sal, pimienta y especias al gusto.\n";
        std::cout << "3. Colocar el pollo en una bandeja para hornear.\n";
        std::cout << "4. Hornear durante 45-60 minutos o hasta que esté bien cocido.\n";
    } else if (comida == "Pescado") {
        std::cout << "Receta para Pescado al Vapor:\n";
        std::cout << "1. Sazonar el pescado con sal, pimienta y jugo de limón.\n";
        std::cout << "2. Colocar el pescado en una vaporera.\n";
        std::cout << "3. Cocinar al vapor durante 10-15 minutos o hasta que esté bien cocido.\n";
    } else if (comida == "Pizza") {
        std::cout << "Receta para Pizza Casera:\n";
        std::cout << "1. Precalentar el horno a 220 grados Celsius.\n";
        std::cout << "2. Extender la masa de pizza en una bandeja para hornear.\n";
        std::cout << "3. Añadir salsa de tomate, queso y tus ingredientes favoritos.\n";
        std::cout << "4. Hornear durante 10-15 minutos o hasta que el queso esté derretido y la masa dorada.\n";
    } else if (comida == "Papas") {
        std::cout << "Receta para Papas Fritas:\n";
        std::cout << "1. Pelar y cortar las papas en tiras.\n";
        std::cout << "2. Calentar aceite en una sartén a fuego medio-alto.\n";
        std::cout << "3. Freír las papas hasta que estén doradas y crujientes.\n";
        std::cout << "4. Escurrir el exceso de aceite y sazonar con sal.\n";
    } else {
        std::cout << "Receta no disponible para " << comida << ".\n";
    }
}