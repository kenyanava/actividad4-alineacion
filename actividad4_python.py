
from enum import Enum

MATRICULA = "250024"
NOMBRE = "Kenya Navarrete Hernandez"


class Estado(Enum):
    INACTIVO = "Inactivo"
    NORMAL = "Normal"
    ALERTA = "Alerta"
    MANTENIMIENTO = "Mantenimiento"


class Lectura:
    def __init__(self, sensor, valor, estado, prioridad=0):
        self.sensor = sensor
        self.valor = valor
        self.estado = estado
        self.prioridad = prioridad


class Monitor:
    def __init__(self, umbral):
        self.umbral = umbral

    def clasificar(self, activo, valor):
        if not activo:
            return Estado.INACTIVO
        elif valor >= self.umbral:
            return Estado.ALERTA
        else:
            return Estado.NORMAL

    def evaluar(self, valor, prioridad):
        activo = True
        alerta = valor >= self.umbral
        prioridad = min(prioridad, 3)
        return activo, alerta, prioridad


def describir_lectura(lectura):
    if lectura.estado == Estado.INACTIVO:
        print(f"{lectura.sensor}: sensor inactivo.")

    elif lectura.estado == Estado.NORMAL:
        print(f"{lectura.sensor}: valor normal de {lectura.valor}.")

    elif lectura.estado == Estado.ALERTA:
        print(
            f"{lectura.sensor}: alerta con valor {lectura.valor} "
            f"y prioridad {lectura.prioridad}."
        )

    elif lectura.estado == Estado.MANTENIMIENTO:
        print(f"{lectura.sensor}: sensor en mantenimiento.")


def mostrar_evaluacion(monitor, nombre, valor, prioridad):
    activo, alerta, prioridad = monitor.evaluar(valor, prioridad)

    print(nombre)
    print(f"Activo: {activo}")
    print(f"Alerta: {alerta}")
    print(f"Prioridad: {prioridad}")
    print()


def main():
    print(f"Matricula: {MATRICULA}")
    print(f"Nombre: {NOMBRE}\n")

    monitor = Monitor(30.0)

    print("=== EVALUACION DE SENSORES ===")

    mostrar_evaluacion(monitor, "Lectura de 27.5 grados", 27.5, 1)
    mostrar_evaluacion(monitor, "Lectura de 34.5 grados", 34.5, 2)

    print("Clasificacion 27.5:", monitor.clasificar(True, 27.5).value)
    print("Clasificacion 34.5:", monitor.clasificar(True, 34.5).value)
    print("Clasificacion inactiva:", monitor.clasificar(False, 34.5).value)

    lectura_normal = Lectura(
        "TEMP-01", 27.5,
        monitor.clasificar(True, 27.5)
    )

    lectura_alerta = Lectura(
        "TEMP-02", 34.5,
        monitor.clasificar(True, 34.5), 2
    )

    lectura_inactiva = Lectura(
        "TEMP-03", 0.0,
        monitor.clasificar(False, 0.0)
    )

    lectura_mantenimiento = Lectura(
        "TEMP-04", 25.0,
        Estado.MANTENIMIENTO
    )

    print("\n=== MODELO DE LECTURAS EN PYTHON ===")

    describir_lectura(lectura_normal)
    describir_lectura(lectura_alerta)
    describir_lectura(lectura_inactiva)
    describir_lectura(lectura_mantenimiento)


if __name__ == "__main__":
    main()
