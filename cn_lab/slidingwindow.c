#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sent = 0, ack = 0, w, f;

/* STOP AND WAIT */
void stop()
{
    sent = 0;
    printf("Enter the number of frames: ");
    scanf("%d", &f);

    while (sent < f)
    {
        printf("Frame %d is transmitted.\n", sent);
        printf("Enter the acknowledgement: ");
        scanf("%d", &ack);
        if (ack == sent)
            sent++;
        else
            printf("Acknowledgement not received properly.\n");
    }
    printf("All frames are transmitted!!!\n");
}
/* GO BACK N */
void goback()
{
    int i;
    sent = 0;
    printf("Enter the window size: ");
    scanf("%d", &w);
    printf("Enter the number of frames: ");
    scanf("%d", &f);
    while (sent < f)
    {
        for (i = 0; i < w && sent < f; i++)
        {
            printf("Frame %d is transmitted.\n", sent);
            sent++;
        }
        printf("Enter the last acknowledgement: ");
        scanf("%d", &ack);

        if (ack == sent - 1)
        {
            printf("All frames are transmitted!!!\n");
        }
        else
        {
            printf("There is a frame with negative acknowledgement.\n");
            sent = ack + 1;
        }
    }
}

/* SELECTIVE REPEAT */
void selective()
{
    int n;
    sent = 0;
    printf("Enter the window size: ");
    scanf("%d", &w);
    printf("Enter the number of frames: ");
    scanf("%d", &f);

    while (sent < f)
    {
        for (n = 0; n < w && sent < f; n++)
        {
            printf("Frame %d is transmitted.\n", sent);
            sent++;
        }
        printf("Enter the frame with no acknowledgement or negative acknowledgement: ");
        scanf("%d", &ack);

        if (ack == -1)
        {
            printf("All frames are transmitted!!!\n");
        }
        else
        {
            printf("Frame %d is retransmitted.\n", ack);
        }
    }
}

/* MAIN FUNCTION */
void main()
{
    int choice;
    while (1)
    {
        printf("\n--- MENU DRIVEN ---\n");
        printf("1. Stop and Wait\n");
        printf("2. Go Back N\n");
        printf("3. Selective Repeat\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            stop();
            break;
        case 2:
            goback();
            break;
        case 3:
            selective();
            break;
        case 4:
            exit(0);
        default:
            printf("Invalid choice\n");
        }
    }
}
