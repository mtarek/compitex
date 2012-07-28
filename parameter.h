enum datatype {
	DTYPE_DOUBLE,
	DTYPE_INT,
	DTYPE_TOT,
};

typedef struct Parameter {
	unsigned int type;
	char *identifier;
} Parameter;

int Parameter_eq(void *d1, void *d2);
Parameter * Parameter_build(unsigned int type, char *id);