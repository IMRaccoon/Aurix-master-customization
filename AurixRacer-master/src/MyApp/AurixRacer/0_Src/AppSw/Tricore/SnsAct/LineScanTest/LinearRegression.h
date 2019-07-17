#define LINEAR_DATA		4
#define LINEAR_DATA_AVG 2.5
#define ERROR_RANGE 	10

#define LEFT_INDEX	80
#define RIGHT_INDEX 43

#define CENTER_INDEX (LEFT_INDEX+RIGHT_INDEX)/2
float32 LinearRegression_mean(float32 a[]);
float32 LinearRegression_sigma(float32 a[]);
void LinearRegression_pop(float32 a[], float32 pop);
void LinearRegression_run(sint32 * l, sint32 * r);
void LinearRegression_setData(sint16 data, boolean isLeft);
