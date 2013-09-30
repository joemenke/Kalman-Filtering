#include <stdio.h>
#include <stdlib.h>

typedef struct Matrix2DF
{
	int rows;
	int cols;
	float ** array;
} matrix2df;


typedef struct Kalman_State_
{
	matrix2df* x; //mean;
	matrix2df* P; //std;
} kalman_state;



void mat_Print(matrix2df* mat)
{
	int i,j;
	for(i = 0; i <mat->rows; i++)
	{
		printf("| ");
		for(j = 0; j<mat->cols; j++)
			printf("%.8f ",mat->array[i][j]);
		printf("|\n");
	}
}

matrix2df* mat_AllocZero( int rows, int cols )
{
	int i,j;
	matrix2df* mat = (matrix2df*)malloc(sizeof(matrix2df));
	mat->rows = rows;
	mat->cols = cols;
	mat->array = (float**)malloc(sizeof(float*)*rows);
	for( i = 0; i<rows; i++)
	{
		mat->array[i] = (float*)malloc(sizeof(float)*cols);
		for(j = 0; j<cols; j++)
			mat->array[i][j] = 0;
	}
	return mat;
}

matrix2df* mat_Alloc( int rows, int cols)
{
	int i;
	matrix2df* mat = (matrix2df*)malloc(sizeof(matrix2df));
	mat->rows = rows;
	mat->cols = cols;
	mat->array = (float**)malloc(sizeof(float*)*rows);
	for( i = 0; i<rows; i++)
	{
		mat->array[i] = (float*)malloc(sizeof(float)*cols);
	}
	return mat;
}

void  mat_Free( matrix2df* mat)
{
	int i;
	for( i = 0; i<mat->rows; i++)
	{
		free(mat->array[i]);
	}
	free(mat->array);
	free(mat);
}

matrix2df* mat_Subtract( matrix2df* mat1, matrix2df* mat2)
{
	if((mat1->rows != mat2->rows) || (mat1->cols != mat2->cols))
	{
		printf("ERROR: Cannot Subtract, Matrix Dimensions Must Agree\n");
		return 0;
	}
	else
	{
		int i, j;
		matrix2df* res = mat_Alloc( mat1->rows, mat1->cols);
		for(i = 0; i< mat1->rows; i++)
			for(j = 0; j < mat1->cols; j++)
				res->array[i][j] = mat1->array[i][j]-mat2->array[i][j];
		return res;
	}
}

matrix2df* mat_Add( matrix2df* mat1, matrix2df* mat2)
{
	if((mat1->rows != mat2->rows) || (mat1->cols != mat2->cols))
	{
		printf("ERROR: Cannot Add, Matrix Dimensions Must Agree\n");
		return 0;
	}
	else
	{
		int i, j;
		matrix2df* res = mat_Alloc( mat1->rows, mat1->cols);
		for(i = 0; i< mat1->rows; i++)
			for(j = 0; j < mat1->cols; j++)
				res->array[i][j] = mat1->array[i][j]+mat2->array[i][j];
		return res;
	}
}

matrix2df* mat_Multiply( matrix2df* mat1, matrix2df* mat2)
{
	if(mat1->cols != mat2->rows)
	{
		printf("ERROR: Cannot Multiply, Inner Matrix Dimensions Must Agree\n");
		return 0;
	}
	else
	{
		int i,j,k;
		matrix2df* res = mat_AllocZero(mat1->rows, mat2->cols);
		for(i = 0; i< mat1->rows; i++)
			for(j = 0 ; j< mat2->cols; j++)
				for(k = 0; k< mat1->cols; k++)
					res->array[i][j]+=mat1->array[i][k]*mat2->array[k][j];
		return res;
	}
}

matrix2df* mat_Transpose( matrix2df* mat)
{
	int i,j;
	matrix2df* res = mat_Alloc(mat->cols,mat->rows);
	for(i = 0; i<mat->rows; i++)
		for(j =0; j<mat->cols; j++)
			res->array[j][i] = mat->array[i][j];
	return res;
}


matrix2df* mat_InverseSimple( matrix2df* mat)
{
	if(mat->cols !=1 || mat->rows!=1)
	{
		printf("ERROR: Matrix needs to be of sise 1 to use simple inverse\n");
		return 0;
	}
	matrix2df* res = mat_Alloc(1,1);
	res->array[0][0] = 1/(mat->array[0][0]);
	return res;
}

void kal_MeasurementUpdate(kalman_state* state, matrix2df*  measurement,
		matrix2df* R /*measurement noise*/, matrix2df* H /*measurement function*/)
{
	int i;
	matrix2df* temp,*temp2,*Ht, *y, *s,*K, *I;
	Ht = mat_Transpose(H);
        //y=measurement-H*x --- Error
        temp = mat_Multiply(H, state->x);
	y = mat_Subtract(measurement,temp);
	mat_Free(temp);
	//printf("Y Done\n");
	//s=H*P*H.transpose()+R ---
	temp = mat_Multiply(H,state->P);
	temp2 = mat_Multiply(temp,Ht);
	mat_Free(temp);
	s = mat_Add(temp2,R);
	mat_Free(temp2);
	//printf("s Done\n");
        //K=P*H.transpose()*s.inverse() --- Kalman Gain
	temp=mat_Multiply(state->P,Ht);
	temp2 = mat_InverseSimple(s);
	mat_Free(s);
	mat_Free(Ht);
	K=mat_Multiply(temp,temp2);
	mat_Free(temp);
	mat_Free(temp2);
	//printf("K Done\n");
        //x=x+(K*y) --- State Update
	temp = mat_Multiply(K,y);
	mat_Free(y);
	temp2 = state->x;
	state->x = mat_Add(state->x, temp);
	mat_Free(temp2);
	mat_Free(temp);
	//printf("x Done\n");
        //P=(I-K*H)*P --- Uncertainty Update
	temp = mat_Multiply(K,H);
	mat_Free(K);
	I = mat_AllocZero(temp->rows,temp->rows);
	for(i = 0; i< (I->rows); i++)
		I->array[i][i] = 1;
	//printf("I Done\n");
	//fflush(stdout);
	temp2 = mat_Subtract(I,temp);
	mat_Free(temp);
	mat_Free(I);
	temp = state->P;
	state->P= mat_Multiply(temp2,state->P);
	mat_Free(temp2);
	mat_Free(temp);
	//printf("P Done\n");
	//fflush(stdout);
	return;
}

void kal_PredictionUpdate(kalman_state * state, matrix2df* F /*State Space Model*/, matrix2df* Q /*Movement Uncertainty*/)
{
	matrix2df* temp,*temp2,* Ft;
	Ft = mat_Transpose(F);
	//x=F*x+u
	temp = state->x;
	state->x = mat_Multiply(F,state->x); //NOTE: We are assuming U (External Motion) is 0;
	mat_Free(temp);
    //P=F*P*F.transpose()+Q
	temp = mat_Multiply(F,state->P);
	temp2 = state->P;
	state->P = mat_Multiply(temp,Ft);
	mat_Free(temp2);
	mat_Free(temp);
	temp = state->P;
	state->P = mat_Add(temp,Q);
	mat_Free(temp);
	mat_Free(Ft);
	return;
}

//int main()
//{
//	kalman_state* state;
//	matrix2df* F,*H,*R, *meas;
//	state = malloc(sizeof(kalman_state));
//	//int i;
//	printf("Testing Program\n");
//	meas = mat_Alloc(1,1);
//	state->x = mat_AllocZero(2,1);
//	state->P = mat_AllocZero(2,2);
//	state->P->array[0][0] = 1000;
//	state->P->array[1][1] = 1000;
//	F = mat_Alloc(2,2);
//	H = mat_Alloc(1,2);
//	R = mat_Alloc(1,1);
//	F->array[0][0] =1;
//	F->array[0][1] =1;
//	F->array[1][1] =1;
//	F->array[1][0] =0;
//	H->array[0][0] =1;
//	H->array[0][1] =0;
//	R->array[0][0] =1;
//	meas->array[0][0]=1;
//	//printf("Init Done\n");
//	//fflush(stdout);
//	kal_MeasurementUpdate(state,meas,R,H);
//	//printf("First Meas Done\n");
//	kal_PredictionUpdate(state,F);
//	printf("x:\n");
//	mat_Print(state->x);
//	printf("P:\n");
//	mat_Print(state->P);
//	meas->array[0][0]=2;
//	kal_MeasurementUpdate(state,meas,R,H);
//	kal_PredictionUpdate(state,F);
//	printf("x:\n");
//	mat_Print(state->x);
//	printf("P:\n");
//	mat_Print(state->P);
//	meas->array[0][0]=3;
//	kal_MeasurementUpdate(state,meas,R,H);
//	kal_PredictionUpdate(state,F);
//	printf("x:\n");
//	mat_Print(state->x);
//	printf("P:\n");
//	mat_Print(state->P);
//	/*matrix2df* mat = mat_AllocZero(2,3);
//	mat->array[1][1] = 6;
//	mat->array[0][0] = 2;
//	mat->array[0][2] = 3;
//	matrix2df* mat2 = mat_AllocZero(3,3);
//	mat2->array[1][1] = 2;
//	mat2->array[1][2] = 3;
//	mat2->array[0][1] = 7;
//	mat2->array[0][2] = 1;
//	matrix2df* s = mat_Add(mat,mat2);
//	matrix2df* m = mat_Multiply(mat,mat2);
//	matrix2df* t = mat_Transpose( m );
//	printf("Mat:\n");
//	mat_Print(mat);
//	printf("Mat2:\n");
//	mat_Print(mat2);
//	printf("m:\n");
//	mat_Print(m);
//	printf("t:\n");
//	mat_Print(t);
//	for( i = 0 ; i<10000; i++)
//	{
//		matrix2df* test = mat_Alloc(1000,1000);
//		mat_Free(test);
//	}*/
//	printf("Finished Testing\n");
//while(1);
//}






//kalman_state* state;
//	matrix2df* F,*H,*R, *meas;
//	state = malloc(sizeof(kalman_state));
//	//int i;
//	printf("Testing Program");
//	meas = mat_Alloc(1,1);
//	state->x = mat_AllocZero(2,1);
//	state->P = mat_AllocZero(2,2);
//	state->P->array[0][0] = 1000;
//	state->P->array[1][1] = 1000;
//	F = mat_Alloc(2,2);
//	H = mat_Alloc(1,2);
//	R = mat_Alloc(1,1);
//	F->array[0][0] =1;
//	F->array[0][1] =1;
//	F->array[1][1] =1;
//	F->array[1][0] =0;
//	H->array[0][0] =1;
//	H->array[0][1] =0;
//	R->array[0][0] =1;
//	meas->array[0][0]=1;
//	//printf("Init Done\n");
//	//fflush(stdout);
//	kal_MeasurementUpdate(state,meas,R,H);
//	//printf("First Meas Done\n");
//	kal_PredictionUpdate(state,F);
//	printf("x:\n");
//	mat_Print(state->x);
//	printf("P:\n");
//	mat_Print(state->P);
//	meas->array[0][0]=2;
//	kal_MeasurementUpdate(state,meas,R,H);
//	kal_PredictionUpdate(state,F);
//	printf("x:\n");
//	mat_Print(state->x);
//	printf("P:\n");
//	mat_Print(state->P);
//	meas->array[0][0]=3;
//	kal_MeasurementUpdate(state,meas,R,H);
//	kal_PredictionUpdate(state,F);
//	printf("x:\n");
//	mat_Print(state->x);
//	printf("P:\n");
//	mat_Print(state->P);
//
//	printf("Finished Testing\n");
