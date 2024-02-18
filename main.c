/*EL2208 Praktikum Pemecahan Masalah dengan C 2021/2022
*Modul : 9 - Tugas Besar
*Kelompok : B5
*Hari dan Tanggal : Jumat, 29 April 2022
*Asisten (NIM) : Agape D’sky (13219010)
*Nama File : main.c
*Deskripsi : mencari jalur pada sebuah graph dengan ketentuan jumlah edges yang 
            ditentukan dari input yang dimasukkan oleh user. Inputnya berupa file eksternal 
            yang berisikan jumlah vertex dan adjacent matrix dari graph yang akan dicari jalurnya, 
            vertex asal, vertex tujuan, dan batas edges yang dimasukkan oleh user, sedangkan output 
            yang diharapkan berupa semua kombinasi jalur pada graph yang ditampilkan pada layar dan 
            file eksternal yang berisi jumlah jalur pada graph dan kombinasi jalur yang ditemukan
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int length;
    char *path;
    int current;
    int prev;
} Path;

Path paths[10];
int M = 0;


typedef struct
{
    int matrix[100][100];
    int row;
    int col;
} Adjacent;

int visited(Path path, int current)
{
    for (int i = 0; i < path.length; i++)
    {
        if (path.path[i] == current + '1')
        {
            return 1;
        }
    }
    return 0;
}

void getPath(Adjacent adjacent, Path path, int origin, int target, int maxEdges)
{   
    if (path.length > maxEdges + 1)
    {
        return;
    }
    if (path.current == target)
    {
        if (path.length == maxEdges + 1)
        {   
            Path newPath;
            newPath.path = (char *)malloc(sizeof(char) * (path.length));
            for (int i = 0; i < path.length; i++)
            {
                newPath.path[i] = path.path[i];
            }
            newPath.length = path.length;
            paths[M] = newPath;
            M += 1;
        }
        return;
    }
    for (int i = 0; i < adjacent.row; i++)
    {
        if (adjacent.matrix[path.current][i] == 1 && i != path.prev && !visited(path, i))
        {
            Path newPath = path;
            newPath.path[newPath.length] = i + '1';
            newPath.length++;
            newPath.prev = newPath.current;
            newPath.current = i;
            getPath(adjacent, newPath, i, target, maxEdges);
        }
    }
}

int main()
{
    //Inisiasi variable
    char buffer[1024];
    char filename[256];
    int i = 0, j = 0;
    int asal, tujuan, batas;
    char *token, *line;
    Adjacent adjacent;

	//Input file eksternal
	printf("Masukkan Graphs: ");
	scanf("%s", filename);
	FILE *fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);

    //Conditional kalau file tidak ada, format salah, kosong
    if (fp == NULL)
    {
        printf("Error: file invalid! File tidak ditemukan.");
        return -1;
    }

    if (ftell(fp) == 0)
    {
        printf("Error: file empty! File kosong.");
        return -1;
    }

    // Mengembalikan pointer ke bagian awal file
    fseek(fp, 0, SEEK_SET);
    
    //Membaca dan menyimpan isi file tiap baris
    char *line1 = fgets(buffer, sizeof(buffer), fp);
    int n = atoi(line1); //banyak vertex

    while ((line = fgets(buffer, sizeof(buffer), fp)) != NULL)    
    {
        j = 0; //kolom
        token = strtok(line, ",");
        while (token != NULL)
        {   
            adjacent.matrix[i][j++] = atoi(token);
            if ((atoi(token) != 0) && (atoi(token) != 1))
            {
                printf("Error: file invalid! Format isi file salah"); //solved file_format_salah6.txt
                return -1;
            }
            token = strtok(NULL, ",");
        }
        ++i;
    }

    //KEMUNGKINAN FILE FORMAT SALAH. asumsi matrix selalu nxn
    // Tidak ada banyak vertex --file_format_salah1.txt
    // Tidak ada adjacent matrix --file_format_salah2.txt
    // Penempatan banyak vertex dan adjacent matrix tidak berurutan --file_format_salah3.txt
    // Pemisah matrix bukan titik koma --file_format_salah4.txt
    // Ukuran adjacent matrix tidak sesuai dengan banyak vertex --file_format_salah5.txt
    // Matrix bukan 0 atau 1 --file_format_salah6.txt

    //solved file_format_salah1.txt
    //solved file_format_salah2.txt
    //solved file_format_salah3.txt
    //solved file_format_salah5.txt
    if ((n!=i) && (n!=j))
    {
        printf("Error: file invalid! Format isi file salah");
        return -1;
    }

    //solved file_format_salah4.txt
    // tidak menemukan batas untuk tiap elemen pada baris, maka 1 baris dianggap 1 elemen
    if (j==1)
    {
        printf("Error: file invalid! Format isi file salah");
        return -1;
    }

    //Input vertex asal, vertex tujuan, dan batas edges
    printf("Masukkan Vertex Asal: ");
    scanf("%d", &asal);

    printf("Masukkan Vertex Tujuan: ");
    scanf("%d", &tujuan);

    printf("Masukkan Batas Edges: ");
    scanf("%d", &batas);


    adjacent.row = i;
    adjacent.col = j;

    asal -= 1;
    tujuan -= 1;

    Path path;
    path.length = 0;
    path.path = (char*) malloc(sizeof(char) * (batas + 1));
    path.path[path.length] = asal + '1';
    path.length++;
    path.current = asal;
    path.prev = asal;

    // Menulis hasil jalur yang ditemui ke file baru
    // Menampilkan ke layar hasil jalur yang ditemui
    char outputfilename[17];
    
    strcpy(outputfilename, "jalur-");
    strcat(outputfilename, filename);
    FILE *fstream_out = fopen(outputfilename, "w");
    getPath(adjacent, path, asal, tujuan, batas);
    free(path.path);
    printf("\nBanyak Jalur: %d\n", M);
    printf("Jalur:\n");

    if (M == 0)
    {
        printf("Tidak Ada Jalur Yang Memenuhi");
        fprintf(fstream_out,"Tidak Ada Jalur Yang Memenuhi");
    }

    else
    {
        for (int k = 0; k < M; k++)
        {
            for (int i = 0; i < paths[k].length; i++)
            {
                if (i == paths[k].length - 1) // Vertex terakhir
                {

                    printf("%c\n", paths[k].path[i]);
                    fprintf(fstream_out,"%c\n", paths[k].path[i]);
                }
                else //Selain terakhir
                {
                    printf("%c->", paths[k].path[i]);
                    fprintf(fstream_out,"%c->", paths[k].path[i]);
                }
            }
        }        
    }
    return 0;
}
