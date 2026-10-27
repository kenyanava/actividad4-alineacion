
const MATRICULA: &str = "10250024";
const NOMBRE: &str = "Kenya Navarrete Hernandez";

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Estado {
    Inactivo,
    Normal,
    Alerta,
    Mantenimiento,
}

struct Lectura {
    sensor: String,
    valor: f64,
    estado: Estado,
    prioridad: u8,
}

struct Monitor {
    umbral: f64,
}

impl Monitor {
    fn nuevo(umbral: f64) -> Self {
        Self { umbral }
    }

    fn clasificar(&self, activo: bool, valor: f64) -> Estado {
        if !activo {
            Estado::Inactivo
        } else if valor >= self.umbral {
            Estado::Alerta
        } else {
            Estado::Normal
        }
    }

    fn evaluar(&self, valor: f64, prioridad: u8) -> (bool, bool, u8) {
        let activo = true;
        let alerta = valor >= self.umbral;
        let prioridad = prioridad.min(3);

        (activo, alerta, prioridad)
    }
}

fn describir_lectura(lectura: &Lectura) {
    match lectura.estado {
        Estado::Inactivo => {
            println!("{}: sensor inactivo.", lectura.sensor);
        }
        Estado::Normal => {
            println!(
                "{}: valor normal de {}.",
                lectura.sensor, lectura.valor
            );
        }
        Estado::Alerta => {
            println!(
                "{}: alerta con valor {} y prioridad {}.",
                lectura.sensor, lectura.valor, lectura.prioridad
            );
        }
        Estado::Mantenimiento => {
            println!("{}: sensor en mantenimiento.", lectura.sensor);
        }
    }
}

fn mostrar_evaluacion(
    monitor: &Monitor,
    nombre: &str,
    valor: f64,
    prioridad: u8,
) {
    let (activo, alerta, prioridad) = monitor.evaluar(valor, prioridad);

    println!("{}", nombre);
    println!("Activo: {}", activo);
    println!("Alerta: {}", alerta);
    println!("Prioridad: {}", prioridad);
    println!();
}

fn main() {
    println!("Matricula: {}", MATRICULA);
    println!("Nombre: {}\n", NOMBRE);

    let monitor = Monitor::nuevo(30.0);

    println!("=== EVALUACION DE SENSORES ===");

    mostrar_evaluacion(&monitor, "Lectura de 27.5 grados", 27.5, 1);
    mostrar_evaluacion(&monitor, "Lectura de 34.5 grados", 34.5, 2);

    println!(
        "Clasificacion 27.5: {:?}",
        monitor.clasificar(true, 27.5)
    );

    println!(
        "Clasificacion 34.5: {:?}",
        monitor.clasificar(true, 34.5)
    );

    println!(
        "Clasificacion inactiva: {:?}\n",
        monitor.clasificar(false, 34.5)
    );

    let lectura_normal = Lectura {
        sensor: String::from("TEMP-01"),
        valor: 27.5,
        estado: monitor.clasificar(true, 27.5),
        prioridad: 0,
    };

    let lectura_alerta = Lectura {
        sensor: String::from("TEMP-02"),
        valor: 34.5,
        estado: monitor.clasificar(true, 34.5),
        prioridad: 2,
    };

    let lectura_inactiva = Lectura {
        sensor: String::from("TEMP-03"),
        valor: 0.0,
        estado: monitor.clasificar(false, 0.0),
        prioridad: 0,
    };

    let lectura_mantenimiento = Lectura {
        sensor: String::from("TEMP-04"),
        valor: 25.0,
        estado: Estado::Mantenimiento,
        prioridad: 0,
    };

    println!("=== MODELO DE LECTURAS EN RUST ===");

    describir_lectura(&lectura_normal);
    describir_lectura(&lectura_alerta);
    describir_lectura(&lectura_inactiva);
    describir_lectura(&lectura_mantenimiento);
}