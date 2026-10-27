#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

constexpr const char* MATRICULA = "10250024";
constexpr const char* NOMBRE = "Kenya Navarrete";

struct RegistroOrdenA {
    char codigo;
    double valor;
    int muestras;
};

struct RegistroOrdenB {
    double valor;
    int muestras;
    char codigo;
};

struct EstadoCompacto {
    unsigned activo : 1;
    unsigned alerta : 1;
    unsigned prioridad : 2;
    unsigned reservado : 4;
};

enum class Estado {
    Inactivo,
    Normal,
    Alerta,
    Mantenimiento
};

struct Lectura {
    std::string sensor;
    double valor;
    Estado estado;
    unsigned prioridad;
};

class Monitor {
private:
    double umbral_;

public:
    explicit Monitor(double umbral) : umbral_(umbral) {
    }

    Estado clasificar(bool activo, double lectura) const {
        if (!activo) {
            return Estado::Inactivo;
        }

        return lectura >= umbral_
            ? Estado::Alerta
            : Estado::Normal;
    }

    EstadoCompacto evaluar(
        double lectura,
        unsigned prioridadSolicitada
    ) const {
        EstadoCompacto estado{};

        estado.activo = 1u;
        estado.alerta = lectura >= umbral_ ? 1u : 0u;
        estado.prioridad =
            prioridadSolicitada <= 3u
                ? prioridadSolicitada
                : 3u;
        estado.reservado = 0u;

        return estado;
    }
};

const char* textoEstado(Estado estado) {
    switch (estado) {
        case Estado::Inactivo:
            return "Inactivo";
        case Estado::Normal:
            return "Normal";
        case Estado::Alerta:
            return "Alerta";
        case Estado::Mantenimiento:
            return "Mantenimiento";
    }

    return "Desconocido";
}

void describirLectura(const Lectura& lectura) {
    std::cout << lectura.sensor
              << ": valor " << lectura.valor
              << " | estado " << textoEstado(lectura.estado)
              << " | prioridad " << lectura.prioridad
              << '\n';
}

void mostrarDisenoDeMemoria() {
    static_assert(std::is_standard_layout_v<RegistroOrdenA>);
    static_assert(std::is_standard_layout_v<RegistroOrdenB>);

    constexpr std::size_t bytesDeMiembros =
        sizeof(char) + sizeof(double) + sizeof(int);

    std::cout << "=== REGISTRO ORDEN A ===\n";
    std::cout << "sizeof: " << sizeof(RegistroOrdenA)
              << " bytes\n";
    std::cout << "alignof: " << alignof(RegistroOrdenA)
              << " bytes\n";
    std::cout << "offset codigo: "
              << offsetof(RegistroOrdenA, codigo) << '\n';
    std::cout << "offset valor: "
              << offsetof(RegistroOrdenA, valor) << '\n';
    std::cout << "offset muestras: "
              << offsetof(RegistroOrdenA, muestras) << '\n';
    std::cout << "relleno total: "
              << sizeof(RegistroOrdenA) - bytesDeMiembros
              << " bytes\n\n";

    std::cout << "=== REGISTRO ORDEN B ===\n";
    std::cout << "sizeof: " << sizeof(RegistroOrdenB)
              << " bytes\n";
    std::cout << "alignof: " << alignof(RegistroOrdenB)
              << " bytes\n";
    std::cout << "offset valor: "
              << offsetof(RegistroOrdenB, valor) << '\n';
    std::cout << "offset muestras: "
              << offsetof(RegistroOrdenB, muestras) << '\n';
    std::cout << "offset codigo: "
              << offsetof(RegistroOrdenB, codigo) << '\n';
    std::cout << "relleno total: "
              << sizeof(RegistroOrdenB) - bytesDeMiembros
              << " bytes\n\n";
}

void mostrarEstado(
    const char* nombre,
    const EstadoCompacto& estado
) {
    std::cout << nombre << '\n';
    std::cout << "activo: " << estado.activo << '\n';
    std::cout << "alerta: " << estado.alerta << '\n';
    std::cout << "prioridad: " << estado.prioridad << '\n';
    std::cout << "reservado: " << estado.reservado
              << "\n\n";
}

std::uint8_t crearRegistroConMascaras(
    bool activo,
    bool alerta,
    std::uint8_t prioridad
) {
    constexpr std::uint8_t MASCARA_ACTIVO = 0x01;
    constexpr std::uint8_t MASCARA_ALERTA = 0x02;
    constexpr std::uint8_t MASCARA_PRIORIDAD = 0x0C;
    constexpr unsigned DESPLAZAMIENTO_PRIORIDAD = 2;

    std::uint8_t registro = 0;

    if (activo) {
        registro = static_cast<std::uint8_t>(
            registro | MASCARA_ACTIVO
        );
    }

    if (alerta) {
        registro = static_cast<std::uint8_t>(
            registro | MASCARA_ALERTA
        );
    }

    prioridad = static_cast<std::uint8_t>(
        prioridad & 0x03u
    );

    registro = static_cast<std::uint8_t>(
        registro |
        static_cast<std::uint8_t>(
            (prioridad << DESPLAZAMIENTO_PRIORIDAD)
            & MASCARA_PRIORIDAD
        )
    );

    return registro;
}

int main() {
    std::cout << "Matricula: " << MATRICULA << '\n';
    std::cout << "Nombre: " << NOMBRE << "\n\n";

    mostrarDisenoDeMemoria();

    std::cout << "=== CAMPOS DE BITS ===\n";
    std::cout << "sizeof(EstadoCompacto): "
              << sizeof(EstadoCompacto)
              << " bytes\n\n";

    Monitor monitor{30.0};

    const EstadoCompacto normal =
        monitor.evaluar(27.5, 1u);

    const EstadoCompacto alerta =
        monitor.evaluar(34.5, 2u);

    mostrarEstado("Lectura de 27.5 grados", normal);
    mostrarEstado("Lectura de 34.5 grados", alerta);

    std::cout << "Clasificacion 27.5: "
              << textoEstado(
                    monitor.clasificar(true, 27.5)
                 ) << '\n';

    std::cout << "Clasificacion 34.5: "
              << textoEstado(
                    monitor.clasificar(true, 34.5)
                 ) << '\n';

    std::cout << "Clasificacion inactiva: "
              << textoEstado(
                    monitor.clasificar(false, 34.5)
                 ) << "\n\n";

    const Lectura lecturaNormal{
        "TEMP-01", 27.5,
        monitor.clasificar(true, 27.5), 0u
    };

    const Lectura lecturaAlerta{
        "TEMP-02", 34.5,
        monitor.clasificar(true, 34.5), 2u
    };

    const Lectura lecturaInactiva{
        "TEMP-03", 0.0,
        monitor.clasificar(false, 0.0), 0u
    };

    const Lectura lecturaMantenimiento{
        "TEMP-04", 25.0,
        Estado::Mantenimiento, 0u
    };

    std::cout << "=== MODELO C++ MODERNO ===\n";
    describirLectura(lecturaNormal);
    describirLectura(lecturaAlerta);
    describirLectura(lecturaInactiva);
    describirLectura(lecturaMantenimiento);
    std::cout << '\n';

    const std::uint8_t registroMascaras =
        crearRegistroConMascaras(true, true, 2u);

    std::cout << "=== MASCARAS EXPLICITAS ===\n";
    std::cout << "activo=1, alerta=1, prioridad=2 -> "
              << std::bitset<8>(registroMascaras)
              << '\n';

    std::cout << "Las posiciones de esta mascara "
                 "fueron elegidas por el programa.\n";

    return 0;
}