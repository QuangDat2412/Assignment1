#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h>
#include <stdint.h> 

typedef struct student
{
    char Name[100];
    float GPA;
    int Toeic;
    int Credits;
    int Tuition;
};

struct student sv = {"Le Quang Dat",2,350,100,15000000};
void* menu();
void* update(void*data);
void* send_email(void*data);
pthread_mutex_t lock;

int main(void) 
{ 
    int n;
    printf("----Chon so lieu muon cap nhat----\n");
    printf("0.Exit\n");
    printf("1.GPA\n");
    printf("2.Toeic\n");
    printf("3.Credits\n");
    printf("4.Tuition\n");
    scanf("%d",&n);
    
    switch(n)
    {
        case 0:
        {
            printf("Khong co lieu cap nhat\n");
            exit(0);
        }    
        case 1:
        {
             printf("Nhap diem GPA:\n"); scanf("%f",&sv.GPA);
            break;
        }
        case 2:
        {
            printf("Nhap diem Toeic:\n"); scanf("%d",&sv.Toeic);
            break;
        }
        case 3:
        {
            printf("Nhap so tin chi qua:\n"); scanf("%d",&sv.Credits);
            break;
        }
        case 4:
        {
             printf("Nhap no hoc phi:\n"); scanf("%d",&sv.Tuition);
             break;
        }
        default:
             break;            
        }    
    int res;
    pthread_t p_thread1;
	pthread_t p_thread2;
	res = pthread_mutex_init(&lock, NULL);
	if (res !=0)
	{
		perror ("Mutex create error");
		exit (EXIT_FAILURE);
	}
	res = pthread_create (&p_thread1, NULL, update, NULL);
	if (res != 0)
	{
		perror ("Thread create error");
		exit (EXIT_FAILURE);
	}
	res = pthread_create (&p_thread2, NULL, send_email, NULL);
	if (res != 0)
	{
		perror ("Thread create error");
		exit (EXIT_FAILURE);
	}
	for (int i = 1; i < 20; i++)
	{
		printf ("Main thread waiting %d second ... \n", i);
		sleep (1);
	}
    pthread_mutex_destroy(&lock); 
  
    return 0; 
} 
void* update (void* data)
{

    pthread_mutex_lock (&lock); 
    printf("\n------------------------------\n");
    printf("Thread 1 lock\n");
    printf("Ho so sinh vien sau khi cap nhat\n ");
    printf("Ho Ten: %s\n",sv.Name);
    printf("GPA: %0.2f\n", sv.GPA);
    printf("Toeic: %d\n", sv.Toeic);
    printf("So tin chi qua: %d\n", sv.Credits);
    printf("No hoc phi: %d d\n", sv.Tuition);
    printf("Thread 1 unlock\n");
    printf("\n------------------------------\n");
    pthread_mutex_unlock(&lock);
    return 0;
}
void* send_email(void* data)
{
    char sv_GPA[50], sv_Toeic[50], sv_Credits[50], sv_Tuition[50], sv_Name[50] ,sv_ThreadID[50];
    pthread_mutex_lock(&lock);
    printf("Sending E-Mail\n");
    printf("Thread 2 lock\n" );
    char cmd[100];  // to hold the command.
    char to[] = "ledat241299@gmail.com"; // email id of the recepient.
    char body[100]="Thong tin cua ban da duoc cap nhat";
    sprintf(sv_ThreadID,"Thread ID:%ld\n",pthread_self());
    sprintf(sv_Name,"Ho Ten:%s \n",sv.Name);
    sprintf(sv_GPA,"GPA: %0.2f \n", sv.GPA);
    sprintf(sv_Toeic,"Toeic:%d \n", sv.Toeic);
    sprintf(sv_Credits, "So tin chi qua:%d \n", sv.Credits);
    sprintf(sv_Tuition, "No hoc phi:%d d\n", sv.Tuition);
    /*strcat(body,sv_ThreadID);
    strcat(body, sv_Name);
    strcat(body, sv_GPA);
    strcat(body, sv_Toeic);
    strcat(body, sv_Credits);
    strcat(body, sv_Tuition);*/
    char tempFile[100];     // name of tempfile.

    strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

    FILE *fp = fopen(tempFile,"w"); // open it for writing.
    fprintf(fp,"%s\n",body);
    fprintf(fp,"%s\n",sv_ThreadID); 
    fprintf(fp,"%s\n",sv_Name); 
    fprintf(fp,"%s\n",sv_GPA); 
    fprintf(fp,"%s\n",sv_Toeic); 
    fprintf(fp,"%s\n",sv_Credits); 
    fprintf(fp,"%s\n",sv_Tuition);        // write body to it.
    fclose(fp);             // close it.

    sprintf(cmd,"sendmail %s < %s",to,tempFile); // prepare command.
    system(cmd);     // execute it.
    printf("Email sent successfully\n");
    printf("Thread 2 unlock.\n");
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}
