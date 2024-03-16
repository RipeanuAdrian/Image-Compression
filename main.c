
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct pixel{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Pixel;//un pixel din matrice
typedef struct tree{
	int x, y;
	int size;
	unsigned char cul_r;
	unsigned char cul_g;
	unsigned char cul_b;
	struct tree *right_up;
	struct tree *left_up;
	struct tree *left_down;
	struct tree *right_down;
}Tree;
//crearea_arborelui
void init_tree( Tree **root, int x, int y, int size ){
	*root = (Tree *)malloc(sizeof( Tree ));
	(*root)->right_up = NULL;
	(*root)->right_down = NULL;
	(*root)->left_up = NULL;
	(*root)->left_down = NULL;
	(*root)->x = x;
	(*root)->y = y;
	(*root)->size = size;
}
Tree* add_node_compresion( int x, int y, int size ){
	//creaza un node cuaternal, un nod intern
	Tree *node = (Tree *)malloc(sizeof(Tree));
	node -> right_down = NULL;
	node -> right_up = NULL;
	node -> left_down = NULL;
	node -> left_up = NULL;
	node -> x = x;
	node -> y = y;
	node -> size = size;
	return node;
}
void divide( int x, int y, Pixel **img, long long int prag, int size, Tree* node ){
	//imparte imaginea in in sferturi
	node -> left_up = add_node_compresion( x, y, size / 2 );
	node -> right_up = add_node_compresion( x, y + size / 2, size / 2 );
	node -> right_down = add_node_compresion( x + size / 2, y + size /2 , size /2 );
	node -> left_down = add_node_compresion( x + size / 2, y, size / 2);
}

int test_coulour( Tree *node, Pixel **img, long long int prag, int size ){
	//in functi aceasta se calculeaza cu ajutorul formulelor daca culoarea este uniforma
	int i, j;
	unsigned char c_r = -1, c_g = -1, c_b = -1, same_shit = 1;
	int x = 1;
	unsigned long long sum_red = 0, sum_green = 0, sum_blue = 0, mean = 0;
	for ( i = node -> x; i < ( node -> x )  + size; i++ ){
		
		for( j = node -> y; j < ( node ->y ) + size; j++){
			//se verifica daca e aceasi culoare in matircea
			//portiunea de zona respectiva
			if( x == 1){
				c_r =  img[i][j].r;
				c_g =  img[i][j].g;
				c_b =  img[i][j].b;
				x = 0;
			}
			else if(( x != 1) &&  ( c_r !=  img[i][j].r || c_g !=  img[i][j].g || c_b !=  img[i][j].b )) {
				same_shit = 12;
				//numai e aceasi culoare
			}
			sum_red = sum_red + img[i][j].r;
			sum_green = sum_green + img[i][j].g;
			sum_blue = sum_blue + img[i][j].b;
		}
	}
	// se calculeaza sumele aritmetice si meanul
	sum_red = sum_red / ( size * size );
	sum_green = sum_green / ( size * size);
	sum_blue = sum_blue / ( size * size );
	for ( i = node -> x; i < (node -> x) + size; i++ )
		for( j = node -> y; j < (node -> y) + size; j++){
			mean = mean + ( sum_red - img[i][j].r ) * ( sum_red - img[i][j].r );
			mean = mean + ( sum_green - img[i][j].g ) * ( sum_green - img[i][j].g );
			mean = mean + ( sum_blue - img[i][j].b ) * ( sum_blue - img[i][j].b );
		}
	mean = mean / ( ( size * size ) * 3 );
	if( mean <= prag ){
		if( same_shit == 12){
		//daca mean <= prag se preia media culorilor din acea zona
		//si culorile diferite
		node ->cul_r = sum_blue;
		node ->cul_g = sum_green;
		node ->cul_b = sum_blue;
		return 1;
}
else if( same_shit != 12){
	//daca mean <= prag se ia o culoare
	//daca culorile sunt diferite 
	node ->cul_r = img [i - 1][j - 1].r;
	node ->cul_g = img [i - 1][j - 1].g;
	node ->cul_b = img [i - 1][j - 1].b;
	return 1;
}}
else {   
	node ->cul_r = 0;
		node ->cul_g = 0;
		node ->cul_b = 0;
	return 0;
}
	
return 0;
}
void compresie(Tree *node, Pixel **img, long long prag ){
	if ( test_coulour( node, img, prag, node -> size ) == 0 ){
		divide( node -> x, node -> y, img, prag, node ->size, node );
		compresie( node -> left_up, img, prag );
		compresie( node -> right_up, img, prag );
		compresie( node -> right_down, img, prag );
		compresie( node -> left_down, img, prag );
	}
}
void creare_matrice( Pixel ***img, int n ){
	int i;
	*img = (Pixel**)malloc(sizeof(Pixel*) * n );
	for( i = 0; i < n; i++){
		(*img)[i] = (Pixel*)malloc( sizeof(Pixel) * n );
	}
}
void citire(int *n, FILE *f, Pixel ***img){
	char *aux;
	int ceva, i, j,m;
	char buffer;
	aux = (char*)malloc(sizeof(char) * 20);
	fgets( aux, 19, f );
	free( aux );
	fscanf( f,"%d", n );
	fscanf( f,"%d", &m );
	fscanf( f,"%d", &ceva);
	fscanf(f,"%c",&buffer);
	printf( "%d %d %d\n", *n, m,  ceva );
	// am nevoie sa retin doar inaltimea pentru ca matricea e patratica
	//se creaaza matricea
	creare_matrice( img, *n );
	for( i = 0; i < (*n); i++ ){
		for( j = 0; j < (*n); j++){
			fread( &((*img)[i][j]), sizeof(Pixel), 1, f );//se citesc pixeli imaginii
		}
	}
	fclose( f );
}
//se calculeaza valorile pentru cerinta 1
void write_compresion_file1( Tree *node, int level, int *nr_leaf, int *max_size_uniform, int *min_level, int *max_level ){
	if( node != NULL){
	//se calclueza nivelul maxim din arborele de compresie in max_level
	//level retine nivelul curent 
		if( level > (*max_level) )
			*max_level = level;
	// in min_level vor retine nicelul cel mai mic din arbore care nu necesita divizare
		if( level < *min_level && node -> left_up == NULL){
			*min_level = level;
	// max_size_uniform retirne cea mai mare marime a unui tablou de pixeli nedivizat
			*max_size_uniform  = node -> size;
		}
		if(!( node ->left_down == NULL && node->left_up == NULL && node->right_down==NULL && node->right_up==NULL)){
			write_compresion_file1( node -> left_up, level + 1, nr_leaf, max_size_uniform, min_level, max_level );
			write_compresion_file1( node -> right_up, level + 1, nr_leaf, max_size_uniform, min_level, max_level );
			write_compresion_file1( node -> right_down, level + 1, nr_leaf, max_size_uniform, min_level, max_level );
			write_compresion_file1( node -> left_down, level + 1, nr_leaf, max_size_uniform, min_level, max_level );
		}
		else (*nr_leaf) = (*nr_leaf) + 1 ;
	}
}
// functie de parcurgere recursiva pe nivele
 void count_leaf( Tree * node , int level, FILE *g, Pixel **img){
	if(node==NULL)return;
	unsigned char ceva;
	if( level == 1){
		if( node ->left_down == NULL && node->left_up == NULL && node->right_down==NULL && node->right_up==NULL){
			ceva = 1;
			fwrite( &ceva, sizeof(unsigned char), 1,  g );
				fwrite( &(node->cul_r), sizeof(unsigned char), 1, g );
				fwrite( &(node->cul_g), sizeof(unsigned char), 1, g );
				fwrite( &(node->cul_b), sizeof(unsigned char), 1, g );
			printf("frunza %u\n",ceva);
		printf("%u %u %u \n", node->cul_r, node->cul_g, node->cul_b);
			
			}
		else {
				ceva = 0;
				fwrite( &ceva, sizeof(unsigned char), 1,  g );
				printf("inauntru %u\n",ceva);
				printf("%u %u %u \n", node->cul_r, node->cul_g, node->cul_b);
			}
			}
	else {

		count_leaf(node->left_up, level - 1,g,img);
		count_leaf(node->right_up, level - 1,g,img);
		count_leaf(node->right_down, level - 1,g,img);
		count_leaf(node->left_down, level - 1,g,img);
	}
	return;
 }
 

void parcurgere(Tree *node, Pixel **img){
	if( node != NULL){
		if( node ->left_down == NULL && node->left_up == NULL && node->right_down==NULL && node->right_up==NULL)
		printf("%d %d %d %u %u %u \n",node ->x, node ->y, node -> size , img[node->x][node->y].r, img[node->x][node->y].g, img[node->x][node->y].b);
		printf("========\n");
		parcurgere(node->left_up,img);
		parcurgere(node->right_up,img);
		parcurgere(node->right_down,img);
		parcurgere(node->left_down,img);
	}
}
void free_tree( Tree *node){
	if( node != NULL){
		free_tree( node -> left_up );
		free_tree( node -> right_up );
		free_tree( node -> right_down );
		free_tree( node -> left_down );
		free(node);
		}
	}

void free_img(Pixel **img ,int n){
	int i;
	for( i = 0; i < n; i++)
		free(img[i]);
	free(img);
}
int main(int argc,const char *argv[]){
	Tree *root = NULL; //arbore de comprosie
	//in f citesc din fiser in g scriu in fisier
	FILE *f, *g;
	Pixel **img;
	int i, n = 0;//linile si coloanele;
	int nr_leaf, max_size_uniform, min_level, max_level;
	long long int prag;
	f = fopen(argv[3], "rb");
	citire( &n, f, &img );
	init_tree( &root, 0, 0, n );
		nr_leaf = 0;
		max_size_uniform = 0;
		min_level = n + 1;
		max_level = -1;
		prag = atoi( argv[2] );
		compresie( root, img, prag );
		write_compresion_file1( root, 1, &nr_leaf, &max_size_uniform, &min_level, &max_level );
		 if( strncmp( argv[1], "-c1" ,  3) == 0 ){
			g = fopen( argv[4], "wt");
		fprintf( g, "%d\n%d\n%d\n", max_level, nr_leaf, max_size_uniform );
		fclose( g );
		 }
		 else if(strncmp( argv[1], "-c2" ,  3 ) == 0){
			g = fopen( argv[4], "wb");
			fwrite(&n,sizeof(unsigned int),1,g);
			for( i = 1; i <= max_level; i++){
				count_leaf(root,i,g,img);
			}
			fclose (g );
		 }
	free_img( img, n );
	free_tree( root );
	return 0;
}