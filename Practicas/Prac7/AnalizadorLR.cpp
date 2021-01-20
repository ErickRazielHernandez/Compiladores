#include<bits/stdc++.h>
using namespace std;

class Gramatica{
private:
	vector<char> terminales;
	vector<char> noTerminales;
	vector<string> producciones;

	void agregarTerminal(char terminal){
		if(buscarTerminal(terminal) >= 0)
			return;
		terminales.push_back(terminal);
	}
	void agregarNoTerminal(char noTerminal){
		if(buscarNoTerminal(noTerminal) >= 0)
			return;
		noTerminales.push_back(noTerminal);
	}
public:
	bool esSimboloTerminal(char simbolo){
		if(simbolo >= 'a' && simbolo <= 'z')
			return true;
		return false;	
	}
	bool esSimboloNoTerminal(char simbolo){
		if(simbolo >= 'A' && simbolo <= 'Z' && simbolo != 'E')
			return true;
		return false;
	}
	void agregarProduccion(string produccion){
		for(char simbolo: produccion){
			if(esSimboloTerminal(simbolo))
				agregarTerminal(simbolo);
			else if(esSimboloNoTerminal(simbolo))
				agregarNoTerminal(simbolo);
		}
		producciones.push_back(produccion);
	}
	int buscarTerminal(char simbolo){
		int i = 0;
		for(i = 0; i < terminales.size(); i++)
			if(terminales[i] == simbolo)
				return i;
		return -1;
	}
	int buscarNoTerminal(char simbolo){
		int i = 0;
		for(i = 0; i < noTerminales.size(); i++)
			if(noTerminales[i] == simbolo)
				return i;
		return -1;
	}
	int cantidadTerminales(){
		return terminales.size();
	}
	int cantidadNoTerminales(){
		return noTerminales.size();
	}
	vector<char> obtenerTerminales(){
		return terminales;
	}
	vector<char> obtenerNoTerminales(){
		return noTerminales;
	}
	vector<string> obtenerProducciones(){
		return producciones;
	}
	int obtenerNumProduccion(string produccion){
		for(int i = 0; i < producciones.size(); i++){
			if(producciones[i] == produccion) return i;
		}
		return -1;
	}
	void ordenarSimbolos(){
		sort(terminales.begin(), terminales.end());
		sort(noTerminales.begin(), noTerminales.end());
	}
};

class ElementoLR0{
public:
	string elemento;
	int posPunto;

	ElementoLR0(){
		elemento = ".";
		posPunto = 0;
	}
	ElementoLR0(string produccion){
		elemento = produccion.substr(0, 1) + "." + produccion.substr(1);
		posPunto = 1;
	}
	int avanzar(){
		if(posPunto < elemento.size() - 1){
			swap(elemento[posPunto], elemento[posPunto + 1]);
			posPunto++;
		}
	}
	bool puedeAvanzar(char simbolo){
		if(posPunto < elemento.size() - 1 && elemento[posPunto + 1] == simbolo) return true;
		return false;
	}
	bool puedeAvanzar(){
		if(posPunto < elemento.size() - 1) return true;
		return false;
	}
};
inline bool operator==(const ElementoLR0 elemento1, const ElementoLR0 elemento2){ return elemento1.elemento == elemento2.elemento; }

class ElementoLR1{
private:
	bool estaEnSimbolosAnticipacion(char simbolo){
		for(char simboloAnticipacion : simbolosAnticipacion){
			if(simboloAnticipacion == simbolo) return true;
		}
		return false;
	}
public:
	string elemento;
	int posPunto;
	vector<char> simbolosAnticipacion;

	ElementoLR1(){
		elemento = ".";
		posPunto = 0;
	}
	ElementoLR1(string produccion, vector<char> simbolosAnticipacion){
		elemento = produccion.substr(0, 1) + "." + produccion.substr(1);
		posPunto = 1;
		this->simbolosAnticipacion = simbolosAnticipacion;
	}
	int avanzar(){
		if(posPunto < elemento.size() - 1){
			swap(elemento[posPunto], elemento[posPunto + 1]);
			posPunto++;
		}
	}
	bool puedeAvanzar(char simbolo){
		if(posPunto < elemento.size() - 1 && elemento[posPunto + 1] == simbolo) return true;
		return false;
	}
	bool puedeAvanzar(){
		if(posPunto < elemento.size() - 1) return true;
		return false;
	}
	bool puedeFusionar(ElementoLR1 elemento){
		return this->elemento == elemento.elemento;
	}
	void fusionarCon(ElementoLR1 elemento){
		for(char simbolo : elemento.simbolosAnticipacion){
			if(!estaEnSimbolosAnticipacion(simbolo)){
				this->simbolosAnticipacion.push_back(simbolo);
			}
		}
	}
};
inline bool operator==(const ElementoLR1 elemento1, const ElementoLR1 elemento2){
	return elemento1.simbolosAnticipacion == elemento2.simbolosAnticipacion && elemento1.elemento == elemento2.elemento;
}

class LectorGramatica{
private:
	Gramatica gramatica;
public:
	Gramatica cargarGramatica(string ruta){
		ifstream archivoEntrada;
		string lineaBuffer = "", produccionBuffer = "";

		archivoEntrada.open(ruta);
	    while (archivoEntrada >> lineaBuffer) {
    		produccionBuffer = "";
	    	for(char simbolo: lineaBuffer){
	    		if((simbolo >= 'A' && simbolo <= 'Z') || (simbolo >= 'a' && simbolo <= 'z'))
	    			produccionBuffer += simbolo;
	    	}
	    	gramatica.agregarProduccion(produccionBuffer);
	    }
	    archivoEntrada.close();
	    gramatica.ordenarSimbolos();
	    return gramatica;
	}
};

class TablaLR{
private:
	vector<vector<string>> tabla;
public:
	vector<string> estados;

	TablaLR(int cantidadNoTerminales, int cantidadTerminales, int cantidadEstados){
		tabla = vector<vector<string>>(cantidadEstados, vector<string>(cantidadTerminales + cantidadNoTerminales, ""));
	}
	TablaLR(){
		tabla = vector<vector<string>>(0);
	}
	void agregarEntrada(string accion, char simbolo, int estado, Gramatica gramatica){
		if(simbolo == '$'){
			tabla[estado][gramatica.cantidadTerminales()] = accion;
		}
		else if(gramatica.esSimboloTerminal(simbolo)){
			tabla[estado][gramatica.buscarTerminal(simbolo)] = accion;
		}
		else if(gramatica.esSimboloNoTerminal(simbolo)){
			tabla[estado][gramatica.buscarNoTerminal(simbolo) + gramatica.cantidadTerminales() + 1] = accion;
		}
	}
	string obtenerEntrada(char simbolo, int estado, Gramatica gramatica){
		if(simbolo == '$'){
			return tabla[estado][gramatica.cantidadTerminales()];
		}
		else if(gramatica.esSimboloTerminal(simbolo)){
			return tabla[estado][gramatica.buscarTerminal(simbolo)];
		}
		else if(gramatica.esSimboloNoTerminal(simbolo)){
			return tabla[estado][gramatica.buscarNoTerminal(simbolo) + gramatica.cantidadTerminales() + 1];
		}
	}
	int obtenerIndiceDeEstado(string estado){
		for(int i = 0; i < estados.size(); i++){
			if(estado == estados[i]) return i;
		}
		return -1;
	}
	void agregarFila(){
		tabla.push_back(vector<string>(tabla[0].size()));
	}
	void fusionarFilas(int fila1, int fila2){
		estados[fila1] = estados[fila1] + "/" + estados[fila2];
		for(int i = 0; i < tabla[fila1].size(); i++){
			tabla[fila1][i] = tabla[fila2][i];
		}
		tabla.erase(tabla.begin() + fila2);
	}
};

class EscritorTablaLR{
public:
	void escribirTablaLR(TablaLR tablaLR, Gramatica gramatica, string ruta){
		ofstream archivoSalida;
		vector<char> terminales, noTerminales;
		terminales = gramatica.obtenerTerminales();
		terminales.push_back('$');
		noTerminales = gramatica.obtenerNoTerminales();
		string lineaBuffer = "";
		string entrada = "";

		archivoSalida.open(ruta);
		for(char terminal: terminales)
			archivoSalida << "\t" << terminal;
		for(char noTerminal: noTerminales)
			archivoSalida << "\t" << noTerminal;
		archivoSalida << endl;

		for(int i = 0; i < tablaLR.estados.size(); i++){
			archivoSalida << tablaLR.estados[i];
			for(char terminal: terminales){
				entrada = tablaLR.obtenerEntrada(terminal, i, gramatica);
				archivoSalida << "\t" << entrada;
			}
			for(char noTerminal: noTerminales){
				entrada = tablaLR.obtenerEntrada(noTerminal, i, gramatica);
				archivoSalida << "\t" << entrada;
			}
			archivoSalida << endl;
		}
	    archivoSalida.close();
	}
};

class CalculadorPrimero{
private:
	int buscarSimboloEnConjunto(vector<char> conjunto, char S){
		int i;
		for(i = 0; i < conjunto.size(); i++)
			if(conjunto[i] == S){
				return i;
			}
		return -1;
	}
	bool contieneEpsilon(vector<char> &conjunto){
		return buscarSimboloEnConjunto(conjunto, 'E') >= 0;
	}
	void quitarEpsilon(vector<char> &conjunto){
		int pos;
		pos = buscarSimboloEnConjunto(conjunto, 'E');
		if(pos >= 0){
			conjunto.erase(conjunto.begin() + pos);
		}
	}
	void combinarConjuntos(vector<char> &conjuntoDestino, vector<char> conjuntoAuxiliar){
		for(char simbolo: conjuntoAuxiliar){
			if(buscarSimboloEnConjunto(conjuntoDestino, simbolo) < 0){
				conjuntoDestino.push_back(simbolo);
			}
		}
	}
public:
	void calcularPrimero(char X, Gramatica gramatica, vector<char> &conjuntoPrimero){
		if(X == 'E' || X == '$' || gramatica.buscarTerminal(X) >= 0){
			conjuntoPrimero.push_back(X);
		}
		else if(gramatica.buscarNoTerminal(X) >= 0){
			for(string produccion: gramatica.obtenerProducciones()){
				if(produccion[0] == X)
					calcularPrimero(produccion.substr(1), gramatica, conjuntoPrimero);
			}
		}
	}
	void calcularPrimero(string alfa, Gramatica gramatica, vector<char> &conjuntoPrimero){
		vector<char> conjuntoAuxiliar;
		int i;
		for(i = 0; i < alfa.size(); i++){
			conjuntoAuxiliar = vector<char>(0);
			calcularPrimero(alfa[i], gramatica, conjuntoAuxiliar);
			if(contieneEpsilon(conjuntoAuxiliar)){
				quitarEpsilon(conjuntoAuxiliar);
				combinarConjuntos(conjuntoPrimero, conjuntoAuxiliar);
			}
			else{
				combinarConjuntos(conjuntoPrimero, conjuntoAuxiliar);
				return;
			}
		}
		conjuntoPrimero.push_back('E');
	}
};

class CalculadorSiguiente{
private:
	int buscarSimboloEnCadena(string alfa, char X){
		int i;
		for(i = 0; i < alfa.size(); i++)
			if(alfa[i] == X)
				return i;
		return -1;
	}
	int buscarSimboloEnCadena(vector<char> alfa, char X){
		int i;
		for(i = 0; i < alfa.size(); i++)
			if(alfa[i] == X)
				return i;
		return -1;
	}
	void combinarConjuntos(vector<char> &conjuntoDestino, vector<char> conjuntoAuxiliar){
		for(char simbolo: conjuntoAuxiliar){
			if(buscarSimboloEnCadena(conjuntoDestino, simbolo) < 0){
				conjuntoDestino.push_back(simbolo);
			}
		}
	}
public:
	void calcularSiguiente(char X, Gramatica gramatica, vector<char> &conjuntoSiguiente){
		int indice = 0;
		vector<char> conjuntoAuxiliar;
		vector<string> producciones;
		CalculadorPrimero calculadorPrimero;
		producciones = gramatica.obtenerProducciones();

		if(producciones[0][0] == X){
			if(buscarSimboloEnCadena(conjuntoSiguiente, '$') < 0)
				conjuntoSiguiente.push_back('$');
		}
		for(string produccion: producciones){
			indice = buscarSimboloEnCadena(produccion.substr(1), X) + 1;
			if(indice == produccion.size() - 1 && produccion[0] != X){
				conjuntoAuxiliar = vector<char>(0);
				calcularSiguiente(produccion[0], gramatica, conjuntoAuxiliar);
				combinarConjuntos(conjuntoSiguiente, conjuntoAuxiliar);
			}
			else if(indice > 0 && indice < produccion.size() - 1){
				conjuntoAuxiliar = vector<char>(0);
				calculadorPrimero.calcularPrimero(produccion.substr(indice + 1), gramatica, conjuntoAuxiliar);
				if(buscarSimboloEnCadena(conjuntoAuxiliar, 'E') < 0){
					combinarConjuntos(conjuntoSiguiente, conjuntoAuxiliar);
				}
				else if(produccion[0] != X){
					conjuntoAuxiliar = vector<char>(0);
					calcularSiguiente(produccion[0], gramatica, conjuntoAuxiliar);
					combinarConjuntos(conjuntoSiguiente, conjuntoAuxiliar);
				}
			}
		}
	}
};

class CalculadorTablaLR0{
private:
	CalculadorPrimero calculadorPrimero;
	CalculadorSiguiente calculadorSiguiente;
	LectorGramatica lectorGramatica;
	EscritorTablaLR escritorTablaLR;
	TablaLR tablaLR;

	void guardarTablaLR(string ruta, Gramatica gramatica){
		escritorTablaLR.escribirTablaLR(tablaLR, gramatica, ruta);
	}
	int buscarSimboloEnConjunto(vector<char> conjunto, char S){
		int i;
		for(i = 0; i < conjunto.size(); i++)
			if(conjunto[i] == S){
				return i;
			}
		return -1;
	}
	bool estaSubconjuntoEnConjuntos(vector<ElementoLR0> &subconjunto, vector<vector<ElementoLR0>> &conjuntos){
		for(vector<ElementoLR0> subconjuntoActual : conjuntos){
			if(subconjuntoActual == subconjunto) return true;
		}
		return false;
	}
	bool estaElementoEnConjunto(ElementoLR0 &elemento, vector<ElementoLR0> &conjunto){
		for(ElementoLR0 elementoActual : conjunto){
			if(elemento == elementoActual) return true;
		}
		return false;
	}
	void cerradura(vector<ElementoLR0> &conjunto, Gramatica gramatica){
		ElementoLR0 elementoActual;
		for(ElementoLR0 elemento : conjunto){
			if(gramatica.buscarNoTerminal(elemento.elemento[elemento.posPunto + 1]) >= 0){
				for(string produccion : gramatica.obtenerProducciones()){
					if(elemento.posPunto < elemento.elemento.size() && elemento.elemento[elemento.posPunto + 1] == produccion[0]){
						elementoActual = ElementoLR0(produccion);
						if(!estaElementoEnConjunto(elementoActual, conjunto)){
							conjunto.push_back(elementoActual);
						}
					}
				}
			}
		}
	}
	int obtenerNumEstado(vector<ElementoLR0> &conjunto, vector<vector<ElementoLR0>> &conjuntos){
		for(int i = 0; i < conjuntos.size(); i++){
			if(conjunto == conjuntos[i]) return i;
		}
		return -1;
	}
	void calcularAccion(int estadoCalculado, char simbolo, int estadoActual, Gramatica gramatica, vector<ElementoLR0> &conjunto, TablaLR &tablaLR, vector<vector<ElementoLR0>> &conjuntos){
		int numeroProduccion;
		vector<string> producciones = gramatica.obtenerProducciones();
		vector<char> siguiente(0);
		string especial = "Z" + producciones[0].substr(0, 1) + ".";
		CalculadorSiguiente calculadorSiguiente;

		for(ElementoLR0 elemento : conjunto){
			if(elemento.elemento == especial){
				tablaLR.agregarEntrada("OK", '$', estadoCalculado, gramatica);
			}
			else if(!elemento.puedeAvanzar()){
				numeroProduccion = gramatica.obtenerNumProduccion(elemento.elemento.substr(0, elemento.elemento.size() - 1)) + 1;
				if(numeroProduccion > 0){
					siguiente = vector<char>(0);
					calculadorSiguiente.calcularSiguiente(elemento.elemento[0], gramatica, siguiente);
					for(int i = 0; i < siguiente.size(); i++){
						tablaLR.agregarEntrada("r" + to_string(numeroProduccion), siguiente[i], estadoCalculado, gramatica);
					}
				}
			}
		}

		if(gramatica.esSimboloTerminal(simbolo)){
			tablaLR.agregarEntrada("d" + to_string(estadoCalculado + 1), simbolo, estadoActual, gramatica);
		}
		else if(gramatica.esSimboloNoTerminal(simbolo)){
			tablaLR.agregarEntrada(to_string(estadoCalculado + 1), simbolo, estadoActual, gramatica);
		}
	}
public:
	void crearTablaLR0(string rutaCarga, string rutaGuardado){
		LectorGramatica lectorGramatica;
		EscritorTablaLR escritorTablaLR;
		Gramatica gramatica;
		gramatica = lectorGramatica.cargarGramatica(rutaCarga);
		vector<vector<ElementoLR0>> conjuntos;
		vector<ElementoLR0> conjuntoActual(0);
		vector<string> producciones = gramatica.obtenerProducciones();
		tablaLR = TablaLR(gramatica.cantidadNoTerminales(), gramatica.cantidadTerminales() + 1, 1);
		int numEstado = 0;
		string produccion = "Z" + producciones[0].substr(0, 1);
		conjuntoActual.push_back(ElementoLR0(produccion));
		cerradura(conjuntoActual, gramatica);
		conjuntos.push_back(conjuntoActual);
		tablaLR.agregarFila();
		while(numEstado < conjuntos.size()){
			tablaLR.estados.push_back(to_string(numEstado + 1));
			for(char noTerminal : gramatica.obtenerNoTerminales()){
				conjuntoActual = vector<ElementoLR0>(0);
				for(ElementoLR0 elemento : conjuntos[numEstado]){
					if(elemento.puedeAvanzar(noTerminal)){
						elemento.avanzar();
						conjuntoActual.push_back(elemento);
					}
				}
				if(!conjuntoActual.empty()){
					cerradura(conjuntoActual, gramatica);
					if(!estaSubconjuntoEnConjuntos(conjuntoActual, conjuntos)){
						conjuntos.push_back(conjuntoActual);
						tablaLR.agregarFila();
					}
					calcularAccion(obtenerNumEstado(conjuntoActual, conjuntos), noTerminal, numEstado, gramatica, conjuntoActual, tablaLR, conjuntos);
				}
			}
			for(char terminal : gramatica.obtenerTerminales()){
				conjuntoActual = vector<ElementoLR0>(0);
				for(ElementoLR0 elemento : conjuntos[numEstado]){
					if(elemento.puedeAvanzar(terminal)){
						elemento.avanzar();
						conjuntoActual.push_back(elemento);
					}
				}
				if(!conjuntoActual.empty()){
					cerradura(conjuntoActual, gramatica);
					if(!estaSubconjuntoEnConjuntos(conjuntoActual, conjuntos)){
						conjuntos.push_back(conjuntoActual);
						tablaLR.agregarFila();
					}
					calcularAccion(obtenerNumEstado(conjuntoActual, conjuntos), terminal, numEstado, gramatica, conjuntoActual, tablaLR, conjuntos);
				}
			}
			numEstado++;
		}
		guardarTablaLR(rutaGuardado, gramatica);
	}
};

class CalculadorTablaLR1{
private:
	CalculadorPrimero calculadorPrimero;
	CalculadorSiguiente calculadorSiguiente;
	LectorGramatica lectorGramatica;
	EscritorTablaLR escritorTablaLR;
	TablaLR tablaLR;
	vector<vector<ElementoLR1>> conjuntos;

	void guardarTablaLR(string ruta, Gramatica gramatica){
		escritorTablaLR.escribirTablaLR(tablaLR, gramatica, ruta);
	}
	int buscarSimboloEnConjunto(vector<char> conjunto, char S){
		int i;
		for(i = 0; i < conjunto.size(); i++)
			if(conjunto[i] == S){
				return i;
			}
		return -1;
	}
	bool estaSubconjuntoEnConjuntos(vector<ElementoLR1> &subconjunto){
		for(vector<ElementoLR1> subconjuntoActual : conjuntos){
			if(subconjuntoActual == subconjunto) return true;
		}
		return false;
	}
	bool estaElementoEnConjunto(ElementoLR1 &elemento, vector<ElementoLR1> &conjunto){
		for(ElementoLR1 elementoActual : conjunto){
			if(elemento == elementoActual) return true;
		}
		return false;
	}
	int puedeSimplificarse(ElementoLR1 &elemento, vector<ElementoLR1> &conjunto){
		for(int i = 0; i < conjunto.size(); i++){
			if(elemento.puedeFusionar(conjunto[i])) return i;
		}
		return -1;
	}
	void cerradura(vector<ElementoLR1> &conjunto, Gramatica gramatica){
		int i, j, tamConjunto = conjunto.size();
		CalculadorPrimero calculadorPrimero;
		ElementoLR1 elementoActual, elemento;
		vector<char> primero;
		for(j = 0; j < tamConjunto; j++){
			elemento = conjunto[j];
			cout << "Elemento: " << elemento.elemento << endl;
			if(gramatica.buscarNoTerminal(elemento.elemento[elemento.posPunto + 1]) >= 0){
				for(string produccion : gramatica.obtenerProducciones()){
					if(elemento.posPunto < elemento.elemento.size() && elemento.elemento[elemento.posPunto + 1] == produccion[0]){
						for(char simboloAnticipacion : elemento.simbolosAnticipacion){
							primero = vector<char>(0);
							calculadorPrimero.calcularPrimero(elemento.elemento.substr(elemento.posPunto + 2) + simboloAnticipacion, gramatica, primero);

							cout << "Primero(" << elemento.elemento.substr(elemento.posPunto + 2) + simboloAnticipacion << ") = {";
							for(char c : primero){
								cout << c << ((c == primero[primero.size()-1])?"":", ");
							}
							cout << "}" << endl;

							elementoActual = ElementoLR1(produccion, primero);
							if(!estaElementoEnConjunto(elementoActual, conjunto)){
								i = puedeSimplificarse(elementoActual, conjunto);
								if(i >= 0) conjunto[i].fusionarCon(elementoActual);
								else conjunto.push_back(elementoActual);
							}
						}
					}
				}
			}
			tamConjunto = conjunto.size();
		}
	}
	int obtenerNumEstado(vector<ElementoLR1> &conjunto){
		for(int i = 0; i < conjuntos.size(); i++){
			if(conjunto == conjuntos[i]) return i;
		}
		return -1;
	}
	void calcularAccion(int estadoCalculado, char simbolo, int estadoActual, Gramatica gramatica, vector<ElementoLR1> &conjunto, TablaLR &tablaLR){
		int numeroProduccion;
		vector<string> producciones = gramatica.obtenerProducciones();
		string especial = "Z" + producciones[0].substr(0, 1) + ".";

		for(ElementoLR1 elemento : conjunto){
			if(elemento.elemento == especial){
				tablaLR.agregarEntrada("OK", '$', estadoCalculado, gramatica);
			}
			else if(!elemento.puedeAvanzar()){
				numeroProduccion = gramatica.obtenerNumProduccion(elemento.elemento.substr(0, elemento.elemento.size() - 1)) + 1;
				if(numeroProduccion > 0){
					for(char simboloAnticipacion : elemento.simbolosAnticipacion){
						tablaLR.agregarEntrada("r" + to_string(numeroProduccion), simboloAnticipacion, estadoCalculado, gramatica);
					}
				}
			}
		}

		if(gramatica.esSimboloTerminal(simbolo)){
			tablaLR.agregarEntrada("d" + to_string(estadoCalculado + 1), simbolo, estadoActual, gramatica);
		}
		else if(gramatica.esSimboloNoTerminal(simbolo)){
			tablaLR.agregarEntrada(to_string(estadoCalculado + 1), simbolo, estadoActual, gramatica);
		}
	}
public:
	void crearTablaLR1(string rutaCarga, string rutaGuardado){
		LectorGramatica lectorGramatica;
		EscritorTablaLR escritorTablaLR;
		Gramatica gramatica;
		gramatica = lectorGramatica.cargarGramatica(rutaCarga);
		vector<ElementoLR1> conjuntoActual(0);
		vector<string> producciones = gramatica.obtenerProducciones();
		tablaLR = TablaLR(gramatica.cantidadNoTerminales(), gramatica.cantidadTerminales() + 1, 1);
		int numEstado = 0;
		string produccion = "Z" + producciones[0].substr(0, 1);
		conjuntoActual.push_back(ElementoLR1(produccion, vector<char>(1, '$')));
		cerradura(conjuntoActual, gramatica);
		conjuntos.push_back(conjuntoActual);
		tablaLR.agregarFila();
		while(numEstado < conjuntos.size()){
			tablaLR.estados.push_back(to_string(numEstado + 1));
			for(char noTerminal : gramatica.obtenerNoTerminales()){
				conjuntoActual = vector<ElementoLR1>(0);
				for(ElementoLR1 elemento : conjuntos[numEstado]){
					if(elemento.puedeAvanzar(noTerminal)){
						elemento.avanzar();
						conjuntoActual.push_back(elemento);
					}
				}
				if(!conjuntoActual.empty()){
					cerradura(conjuntoActual, gramatica);

					//Imprimir el kernel
					cout << "(" << numEstado << "," << noTerminal << ") = {";
					for(ElementoLR1 e : conjuntoActual){
						cout << "(" << e.elemento << ", ";
						for(int i = 0; i < e.simbolosAnticipacion.size(); i++){
							cout << e.simbolosAnticipacion[i] << ((i == e.simbolosAnticipacion.size()-1)?"":"/");
						}
						cout << ((e == conjuntoActual[conjuntoActual.size()-1])?")":"), ");
					}
					cout << "}" << endl;

					if(!estaSubconjuntoEnConjuntos(conjuntoActual)){
						conjuntos.push_back(conjuntoActual);
						tablaLR.agregarFila();
					}
					calcularAccion(obtenerNumEstado(conjuntoActual), noTerminal, numEstado, gramatica, conjuntoActual, tablaLR);
				}
			}
			for(char terminal : gramatica.obtenerTerminales()){
				conjuntoActual = vector<ElementoLR1>(0);
				for(ElementoLR1 elemento : conjuntos[numEstado]){
					if(elemento.puedeAvanzar(terminal)){
						elemento.avanzar();
						conjuntoActual.push_back(elemento);
					}
				}
				if(!conjuntoActual.empty()){
					cerradura(conjuntoActual, gramatica);

					//Imprimir el kernel
					cout << "(" << numEstado << "," << terminal << ") = {";
					for(ElementoLR1 e : conjuntoActual){
						cout << "(" << e.elemento << ", ";
						for(int i = 0; i < e.simbolosAnticipacion.size(); i++){
							cout << e.simbolosAnticipacion[i] << ((i == e.simbolosAnticipacion.size()-1)?"":"/");
						}
						cout << ((e == conjuntoActual[conjuntoActual.size()-1])?")":"), ");
					}
					cout << "}" << endl;

					if(!estaSubconjuntoEnConjuntos(conjuntoActual)){
						conjuntos.push_back(conjuntoActual);
						tablaLR.agregarFila();
					}
					calcularAccion(obtenerNumEstado(conjuntoActual), terminal, numEstado, gramatica, conjuntoActual, tablaLR);
				}
			}
			numEstado++;
		}
		guardarTablaLR(rutaGuardado, gramatica);
	}
	TablaLR obtenerTablaLR1(){
		return tablaLR;
	}
	vector<vector<ElementoLR1>> obtenerConjuntos(){
		return conjuntos;
	}
};

class CalculadorTablaLALR{
private:
	TablaLR tablaLALR;
	vector<vector<ElementoLR1>> conjuntosLR1;

	bool puedenFusionarse(int conjunto1, int conjunto2){
		if(conjuntosLR1[conjunto1].size() != conjuntosLR1[conjunto2].size()) return false;
		for(int i = 0; i < conjuntosLR1[conjunto1].size(); i++){
			if(!conjuntosLR1[conjunto1][i].puedeFusionar(conjuntosLR1[conjunto2][i])) return false;
		}
		return true;
	}
	void fuisionarEstados(int estado1, int estado2){
		for(int i = 0; i < conjuntosLR1[estado1].size(); i++){
			conjuntosLR1[estado1][i].fusionarCon(conjuntosLR1[estado2][i]);
		}
		conjuntosLR1.erase(conjuntosLR1.begin() + estado2);
		tablaLALR.fusionarFilas(estado1, estado2);
	}
	void comprimirTablaLR1(){
		int tamConjuntos = conjuntosLR1.size(), i = 0;
		while(i < tamConjuntos){
			for(int j = i + 1; j < conjuntosLR1[i].size(); j++){
				if(puedenFusionarse(i, j)){
					fuisionarEstados(i, j);
					i--;
				}
			}
			tamConjuntos = conjuntosLR1.size();
			i++;
		}
	}
public:
	void crearTablaLALR(string rutaCarga, string rutaGuardado){
		CalculadorTablaLR1 calculadorTablaLR1;
		EscritorTablaLR escritorTablaLR;
		LectorGramatica lectorGramatica;
		calculadorTablaLR1.crearTablaLR1(rutaCarga, rutaGuardado);
		tablaLALR = calculadorTablaLR1.obtenerTablaLR1();
		conjuntosLR1 = calculadorTablaLR1.obtenerConjuntos();
		comprimirTablaLR1();
		escritorTablaLR.escribirTablaLR(tablaLALR, lectorGramatica.cargarGramatica(rutaCarga), rutaGuardado);
	}
};