#include "header.h"
uint32_t sourcePort;
void myheadercreater(char* h,uint64_t seq,uint64_t ack,uint32_t _flag);

int main(){

    srand( time(NULL) );
    sourcePort=(uint32_t)(rand()%20000+15000);
    printf("my source port is : %d \n",sourcePort);
    /*---------------------------UDT SERVER----------------------------------*/
    srand(getpid());
        //Create socket.
    int socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        printf("Create socket fail!\n");
        return -1;
    }

    //Set up server's address.
    struct sockaddr_in serverAddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(45525)
    };
    int server_len = sizeof(serverAddr);

    //Connect to server's socket.
    if (connect(socket_fd, (struct sockaddr *)&serverAddr, server_len) == -1) {
        printf("Connect server failed!\n");
        close(socket_fd);
        exit(0);
    }
    /*---------------------------UDT SERVER-----------------------------------*/
    
    

    /*---------------------------3 way hand shake-----------------------------*/
    /*                                                                        */                                              
    /* TODO: Make a three way handshake with RDT server by using TCP header   */
    /*       char header[20] (lab2).                                          */
    /*       Make sure the SrcPort(Random), DesPort(45525), Seg#, Ack#, FLAG  */
    /*       are correct.                                                     */
    /*                                                                        */                                              
    /*---------------------------3 way hand shake-----------------------------*/

//====================task1==============================
    char header[20];
    myheadercreater(header,123,0,SYN);
    // hand shake 1
    send(socket_fd, (void*)header, sizeof(header), 0);
    // hand shack 2
    recv(socket_fd,(void*)header,sizeof(header), 0);

    
    uint64_t byte1,byte2,byte3,byte4;
    // setting seq #
    byte1=header[4]&0xFF;
    byte2=header[5]&0xFF;
    byte3=header[6]&0xFF;
    byte4=header[7]&0xFF;
    uint64_t rev_seq = (byte1<<24)+(byte2<<16)+(byte3<<8)+byte4;
    // setting ack #
    byte1=header[8]&0xFF;
    byte2=header[9]&0xFF;
    byte3=header[10]&0xFF;
    byte4=header[11]&0xFF;
    uint64_t rev_ack=(byte1<<24)+(byte2<<16)+(byte3<<8)+byte4;

    
    myheadercreater(header,rev_ack,rev_seq+1,ACK);

    send(socket_fd, (void*)header, sizeof(header), 0);
//====================task1==============================

//====================task2==============================
    FILE* file = fopen("received_image.jpg", "wb");
    if (file == NULL) {
        perror("Fail to open");
        exit(1);
    }

    /*Segment s;
    char rev_buf[20];
    uint64_t prev_seq=rev_seq;
    int incre_seq;
    
    while( recv(socket_fd,(void*)rev_buf,sizeof(rev_buf), 0)!=-1){
        // setting seq #
        byte1=header[4]&0xFF;
        byte2=header[5]&0xFF;
        byte3=header[6]&0xFF;
        byte4=header[7]&0xFF;
        rev_seq=(byte1<<24)+(byte2<<16)+(byte3<<8)+byte4;
        // setting ack #
        byte1=header[8]&0xFF;
        byte2=header[9]&0xFF;
        byte3=header[10]&0xFF;
        byte4=header[11]&0xFF;
        rev_ack=(byte1<<24)+(byte2<<16)+(byte3<<8)+byte4;
        incre_seq=(rev_seq==prev_seq)?0:1;
        if(incre_seq==0){
            printf("receive 0\n");
            break;
        }

        printf("incre %d\n",incre_seq);

        printf("seq %lu, ACK  %lu\n",rev_seq,rev_ack);
        myheadercreater(header,rev_ack,rev_seq+incre_seq,ACK);
        send(socket_fd, (void*)header, sizeof(header), 0);
        prev_seq=rev_seq;
    }*/
    //=====================task2=============================
}

    /*----------------------------receive data--------------------------------*/
    /*                                                                        */                                              
    /* TODO: Receive data from the RDT server.                                */
    /*       Each packet will be 20bytes TCP header + 1000bytes paylaod       */
    /*       exclude the last one. (the payload may not be exactly 1000bytes) */
    /*                                                                        */
    /* TODO: Once you receive the packets, you should check whether it's      */                                                            
    /*       corrupt or not (checksum) , and send the corresponding ack       */                                                  
    /*       packet (also a char[20] ) back to the server.                    */
    /*                                                                        */
    /* TODO: fwrite the payload into a .jpg file, and check the picture.      */
    /*                                                                        */                                              
    /*----------------------------receive data--------------------------------*/



    /*-------------------------Something important----------------------------*/
    /* NOTE: TO make lab3 simple                                              */
    /*                                                                        */                                              
    /*       1. The SrcIP and DesIP are both 127.0.0.1,                       */
    /*          Tcp header length will be 20byts, windowsize = 65535 bytes    */                                              
    /*       2. The Handshake packets won't be corrupt.                       */
    /*       3. The packet will only corrupt but not miss or be disordered.   */                                              
    /*       4. Only the packets come from server may corrupt.(don't have to  */
    /*          worry that the ack sent by client will corrupt.)              */
    /*       5. We offer mychecksum() for you to verify the checksum, and     */
    /*          don't forget to verify pseudoheader part.                     */
    /*       6. Once server finish transmit the file, it will close the       */
    /*          client socket.                                                */                                              
    /*       7. You can adjust server by                                      */                                              
    /*          ./server {timeout duration} {corrupt probability}             */                                              
    /*                                                                        */                                              
    /*-------------------------Something important----------------------------*/

    void myheadercreater(char* header, uint64_t seq, uint64_t ack, uint32_t _flag){

        // setting source port
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
        uint8_t byte4;
        uint8_t byte5;
        uint8_t byte6;
        uint8_t byte7;
        uint8_t byte8;
        uint8_t byte9;
        uint8_t byte10;
        uint8_t byte11;
        uint8_t byte14;
        uint8_t byte15;
        
        byte0 = sourcePort >> 8;
        byte1 = sourcePort & 0x00FF;
        header[0] = byte0;
        header[1] = byte1;
        // setting dest port
        byte2 = SERVER_PORT>>8;
        byte3 = SERVER_PORT&0x00FF;
        header[2] = byte2;
        header[3] = byte3;
        // setting seqnum
        byte4 = (seq & 0xFF000000)>>24;
        byte5 = (seq & 0x00FF0000)>>16;
        byte6 = (seq & 0x0000FF00)>>8;
        byte7 = (seq & 0x000000FF);
        header[4] = byte4;
        header[5]= byte5;
        header[6]= byte6;
        header[7]= byte7;
        //setting ack num
        
        byte8 = (ack& 0xFF000000)>>24;
        byte9 = (ack& 0x00FF0000)>>16;
        byte10 = (ack& 0x0000FF00)>>8;
        byte11 = (ack& 0x000000FF);
        header[8] = byte8;
        header[9]=byte9;
        header[10]=byte10;
        header[11]=byte11;
        //setting some bit
        header[12]= 0x50;
        header[13] = _flag;
        //setting window size
        uint16_t win = 65535;
        byte14 = win >> 8;
        byte15 = win & 0x00FF;
        header[14] = byte14;
        header[15] = byte15;
    }

