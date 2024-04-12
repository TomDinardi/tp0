#include "client.h"

int main(void) {
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	char* err_message;
	err_message = malloc(sizeof(char) * 50);

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	if( logger == NULL ){

		strcpy(err_message, "Error al crear el logger.");
		free(err_message);
		exit(EXIT_FAILURE);
	}

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");
	
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	if( config == NULL ) {

		strcpy(err_message, "Error al crear el archivo de configuración.");
		log_info(logger, err_message);
		free(err_message);
		exit(EXIT_FAILURE);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = get_config_value(logger, config, "CLAVE");
	ip = get_config_value(logger, config, "IP");
	puerto = get_config_value(logger, config, "PUERTO");

	// Loggeamos el valor de config
	//log_info(logger, valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	//leer_consola(logger);

	
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger() {

	t_log* nuevo_logger;
	t_log_level level;
	bool is_active_console;
	char* process_name = malloc(sizeof(char) * 40);
	char* file = malloc(sizeof(char) * 40);

	strcpy(file, "./tp0.log");
	strcpy(process_name, "TP0");

	is_active_console = true;
	level = LOG_LEVEL_INFO;

	nuevo_logger = log_create(file, process_name, is_active_console, level);

	free(process_name);
	free(file);
	return nuevo_logger;
}

t_config* iniciar_config() {

	t_config* nuevo_config;
	char* path = malloc(sizeof(char) * 40);

	strcpy(path, "../cliente.config");
	nuevo_config = config_create(path);

	free(path);
	return nuevo_config;
}

void leer_consola(t_log* logger) {

	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while( (*leido) != '\0' ) {

		log_info(logger, leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion) {
	// Ahora toca lo divertido!
	char* leido;
	int size_leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	printf("Escriba un mensaje a empaquetar: ");
	leido = readline("> ");
	
	while( (*leido) != '\0' ) {

		size_leido = strlen(leido) +1;
		agregar_a_paquete(paquete, leido, size_leido);
		printf("Escriba lo siguiente a empaquetar: ");
		leido = readline("> ");
	}

	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
}

char* get_config_value(t_log* logger, t_config* config, char* key) {

	char* value;

	if( config_has_property(config, key) ) {

		value = config_get_string_value(config, key);
		log_info(logger, value);
		return value;
	} else {

		log_info(logger, "Error. No se encontró el campo buscado.");
		exit(EXIT_FAILURE);
	}
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	
	if( logger != NULL ) {

		log_destroy(logger);
	}

	if ( config != NULL) {

		config_destroy(config);
	}

	if (conexion != NULL) {

		liberar_conexion(conexion);
	}
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}