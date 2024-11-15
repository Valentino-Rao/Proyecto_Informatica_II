#include "nutricion.h"


int main() {
    SerialPort serial_port("/dev/ttyACM1", B9600); // Asegúrate de que el nombre del puerto sea correcto
    std::ofstream outfile("datos.txt", std::ios::app); // Abre el archivo en modo append
    if (!outfile.is_open()) {
        std::cerr << "Error al abrir el archivo para escribir" << std::endl;
        return 1;
    }

    // Ingresar los valores de consumo diario de la persona
    float calorias_diarias, proteinas_diarias, carbohidratos_diarios, lipidos_diarios;
    std::cout << "Ingrese las calorias diarias: ";
    std::cin >> calorias_diarias;
    std::cout << "Ingrese las proteinas diarias: ";
    std::cin >> proteinas_diarias;
    std::cout << "Ingrese los carbohidratos diarios: ";
    std::cin >> carbohidratos_diarios;
    std::cout << "Ingrese los lipidos diarios: ";
    std::cin >> lipidos_diarios;

    Persona persona(calorias_diarias, proteinas_diarias, carbohidratos_diarios, lipidos_diarios);

    std::string data;
    int mealCount = 0;
    while (mealCount < 3) {
        data = serial_port.readData();
        if (!data.empty()) {
            std::cout << data << std::endl;
            outfile << data << std::endl; // Escribe los datos en el archivo
            mealCount++;
        }
        usleep(100000); // Espera 100ms antes de leer nuevamente
    }

    outfile.close(); // Cierra el archivo

    // Leer los datos del archivo y procesarlos
    std::vector<FoodData> foodDataList = readDataFromFile("datos.txt");

    // Calcular el total de nutrientes consumidos
    float total_calorias = 0, total_proteinas = 0, total_carbohidratos = 0, total_lipidos = 0;
    for (const auto& foodData : foodDataList) {
        NutritionalInfo info = calculateNutritionalInfo(foodData.food, foodData.weight);
        total_calorias += info.calorias;
        total_proteinas += info.proteinas;
        total_carbohidratos += info.carbohidratos;
        total_lipidos += info.lipidos;

        //Imprmir receta
        imprimirReceta(foodData.food);
    }

    // Comparar con los valores preestablecidos
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total calorias consumidas: " << total_calorias << " / " << persona.getCaloriasDiarias() << std::endl;
    std::cout << "Total proteinas consumidas: " << total_proteinas << " / " << persona.getProteinasDiarias() << std::endl;
    std::cout << "Total carbohidratos consumidos: " << total_carbohidratos << " / " << persona.getCarbohidratosDiarios() << std::endl;
    std::cout << "Total lipidos consumidos: " << total_lipidos << " / " << persona.getLipidosDiarios() << std::endl;

    return 0;
}