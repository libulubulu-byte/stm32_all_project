#include "tcp.h"
#include "main.h"
#include "string.h"
#include "stdlib.h"



char Register_data_Tx[290] = {0};
char Register_data_Tx_1[290] = {0};
char Crc16_CCITT[2] = {0};
char Tcp_header[61] = {"HK0100000000000000000000000000000000000000000000111100000000"};
char Heart_Tcp_Tx[104] = {"HK0100000000000000000000000000000000000000000000111100000000vju2cz2Oo5RwnsUAX6FMLdMGGiAJfdQ2DObTiBBE6FE="};
//char Capabilities_TCP_Tx[232] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH5AV8z1ItlZUnzkoGEjJ++rqutat+1f0wD/LTRcpvOhZcfahper4d9BS3e0Ir1nLlfCL7oFPEe9z450igtCu1PGV7FYcte/eRtG/0pvXzMcm0uNho/AreAFOBzGY+W8CW6YZMH0f8D+rowGekBwK5Ss="};
char Capabilities_TCP_Tx[232] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH5AV8z1ItlZUnzkoGEjJ++rqutat+1f0wD/LTRcpvOhZcfahper4d9BS3e0Ir1nLlXhxrxgtXL0upEpU9cHv/MaV7FYcte/eRtG/0pvXzMcm0uNho/AreAFOBzGY+W8CW6YZMH0f8D+rowGekBwK5Ss="};
char Query_Tcp_Tx_A[232] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH6noIJLz0joBOtVOCp8CNBciDwAn2Rz+NdZpRJI/pTT4hH3iR5KipMdociCKXD0saIr+C/rdPI88smjyv6qglc5yuuHFG4LbZi2UM85IP26w7dacUk7IOEPPkTyUkT6iWSl2UGVXOQlIAYNqCiHREPs="};
char Query_Tcp_Tx_B[232] = {"HK0110032020049226BE4334000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH6noIJLz0joBOtVOCp8CNBciDwAn2Rz+NdZpRJI/pTT4hH3iR5KipMdociCKXD0saIr+C/rdPI88smjyv6qglc5yuuHFG4LbZi2UM85IP26w7dacUk7IOEPPkTyUkT6iWSl2UGVXOQlIAYNqCiHREPs="};

unsigned char Query_flag = 0x00;
unsigned char Capabilities_flag = 0x00;
uint8_t pKey[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0x01, 0x23, 0x45, 0x67, 0x89, 0x01, 0x23, 0x45, 0x67, 0x89, 0x12};
uint32_t byKeyLen = 0;
char  data[400] = {0};
//char Heart_data_Tx[40]={0};
// char Heart_data_Tx[35] = {"{\"Commannd\":\"COM_HEARTBEAT\"}"};

unsigned char Register_flag = 0x00;
unsigned char Heart_flag = 0x00;
unsigned char Clearalarm_flag=0x00;
unsigned char FQCF_flag = 0x00;
unsigned char FQBF_flag = 0x00;
unsigned char Fckg_flag = 0x00;
unsigned char FQBJ_flag_1 = 0x00;
unsigned char FQBJ_flag_2 = 0x00;
unsigned char FQBJ_flag_3 = 0x00;
unsigned char FQBJ_flag_4 = 0x00;
unsigned char FQBJ_flag_5 = 0x00;
unsigned char FQBJ_flag_6 = 0x00;
unsigned char FQBJ_flag_7 = 0x00;
unsigned char FQBJ_flag_8 = 0x00;
unsigned char FQBJ_flag_9 = 0x00;
unsigned char FQBJ_flag_A = 0x00;
unsigned char FQBJ_flag_B = 0x00;
unsigned char FQBJ_flag_C = 0x00;
// unsigned char Fckg_flag1 = 0x00;
// unsigned char Fckg_flag2 = 0x00;

//��λ���������͸���λ��������
//char Command_Register[] = {"COM_DEV_REGISTER"};//ע������

char Command_Register[] = {"CrBFXv2EfsBOnSa6pVu+QCxXHDOAZoOFNzt7PAbwFQOsroE0OjzLIfKs6m+e/h0JDBu9fhy4XUnSpyHMMtk/n+HsRuGz5B4utgKeaNFpWKQba5Z34acVRVenXu/RmwHR"};//ע������
char Command_Result[] = {"Result"};
char Command_Heart[] = {"COM_HEARTBEAT"}; //������
char Command_Upload[] = {"COM_UPLOAD_ZONE_ALARM"}; //�豸״̬�ϱ�
// char Upload_FC_Tx_1[81] = {"{\"Command\":\"COM_UPLOAD_DEVICE_ALARM\",\"Data\":{\"deviceType\":\"0001\"}}"};
// char Upload_FC_Tx[130] = {0};
// char Upload_FC_Tcp_header[61] = {"HK0100000000000000000000000000000000000000000000111100000000"};
char Upload_FCBJ_Tcp_Tx[149] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKHzODE4MfKOD5KNQS1aeiYrrzqe0FzH65DuSyrmLLzmiNkrvsoeBCXp0HWPIGOV/b4A=="};
char Upload_FCHF_Tcp_Tx[149] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKHzODE4MfKOD5KNQS1aeiYrrzqe0FzH65DuSyrmLLzmiNQ0WRB3tGMBE5aP+0eyP6/A=="};

char Upload_Alarm_Tcp_Tx_1_1[169] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH00uBnkCnK44vxy8MAdKgZoJRzAkb03QTETztZ0FLfbY0hKkDHNaLfD/Oxe9oj/oAWiziE91FD1RDTjsRWADiWQ="};
char Upload_Alarm_Tcp_Tx_2_1[169] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH00uBnkCnK44vxy8MAdKgZoJRzAkb03QTETztZ0FLfbYqEpnqMASOgmL9UTDB0hVMmiziE91FD1RDTjsRWADiWQ="};
char Upload_Alarm_Tcp_Tx_3_1[169] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH00uBnkCnK44vxy8MAdKgZoJRzAkb03QTETztZ0FLfbY2Hpx7iagcPlq1tC37dD0+GiziE91FD1RDTjsRWADiWQ="};
char Upload_Alarm_Tcp_Tx_4_1[169] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH00uBnkCnK44vxy8MAdKgZoJRzAkb03QTETztZ0FLfbYSKnRcT4QOK+NJ1iymDAn32iziE91FD1RDTjsRWADiWQ="};

char Query_Tcp_Tx_1_1_1[213] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH6noIJLz0joBOtVOCp8CNBciDwAn2Rz+NdZpRJI/pTT4kjWerEIFxibWJ6oO3cIsQ+bgYpjoePqgY2XnMdLRHZhM+aitc9aCBoJGJnSbNjhkXUVgrbejWGFYISx2srTqkg=="};
char Query_Tcp_Tx_1_1_0[213] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH6noIJLz0joBOtVOCp8CNBciDwAn2Rz+NdZpRJI/pTT4kjWerEIFxibWJ6oO3cIsQ+bgYpjoePqgY2XnMdLRHZgcyagTwvhMBt+PND5WfG+FXUVgrbejWGFYISx2srTqkg=="};
char Query_Tcp_Tx_1_0_0[213] = {"HK0100000000000000000000000000000000000000000000111100000000wzCSbNo9nCgekK3pH4fKH6noIJLz0joBOtVOCp8CNBciDwAn2Rz+NdZpRJI/pTT4kjWerEIFxibWJ6oO3cIsQ8EWXIY/A/tqDl/T10TkFjkcyagTwvhMBt+PND5WfG+FXUVgrbejWGFYISx2srTqkg=="};
//��λ���������͸���λ��������
// char Command_Capabilities[] = {"COM_HOST_CAPABILITIES"}; //�豸������
char Command_Capabilities[] = {"CrBFXv2EfsBOnSa6pVu+QOFWnSajU6qo3Zaj9owgztV4OorhJPtwq0nTzxw947Ze"}; //�豸������
char Command_Setuparm_BF[] = {"CrBFXv2EfsBOnSa6pVu+QHNetT9v68J2/J5s5jrzwDPkrIkE9dLMCOpqKPA3EfqnyJRmX1H+E/6qXemoB3EwjyFMC2HRAGznOwzzVquXbQ/QRoKj9YYWmaiLZIXW38GJ"}; //���������Ĳ���
char Command_Setuparm_CF[] = {"CrBFXv2EfsBOnSa6pVu+QHNetT9v68J2/J5s5jrzwDPkrIkE9dLMCOpqKPA3EfqnS9iK+tqXDyb5nOri50s3NiFMC2HRAGznOwzzVquXbQ/QRoKj9YYWmaiLZIXW38GJ"}; //���������ĳ���
char Command_Query[] = {"CrBFXv2EfsBOnSa6pVu+QGy4gOS0uWXqAn0CCQLODWz0jrLE0PesN+VjLAuoY7Rs077H1lGxGHx1p61/IqBAxUt4iqNpJEnkZmV12VTOCp8WjDbU3g2XPCMkpzWrpS1U"}; //�豸״̬��ѯ
char Command_Setuparm[] = {"COM_ZONE_SETUPARM"}; //���������ĳ�������
char Command_Enable[] = {"COM_ZONE_ENABLE"}; //�������ò���ʹ��
char Command_Clearalarm[] = {"CrBFXv2EfsBOnSa6pVu+QHdbnX0LrFB/ouiIxcMqZ9ktrkWF7yHD3VgxsBAyy8nHhc+sTASN8iS+OZkHCFDaEzR1"}; //������������
char Command_Alarmtime[] = {"COM_ZONE_ALARM_TIME"}; //��������ʱ������
//char Command_Query[] = {"COM_QUERY_DEV_STATUS"}; //�豸״̬��ѯ
char Mac_address[13] = {0}; //MAC��ַ
char Mac_address_init[18] = {0};
char Ip_address[16] = {0};
char Receive_Flag = 0x00;

char Deal_data_Header[60] = {0};
char Deal_data[Deal_data_MAX] = {0};
char Deal_data_out[Deal_data_MAX] = {0};
uint16_t Heart_time_Count = 0;
uint16_t Register_data_Count = 0;

uint16_t Register_init_Count=0;
char Register_data_flag=0x01;

//we need change this data 20210409
void Tcp_Header_init(void)
{
    Heart_Tcp_Tx[52] = 0x00;
    Heart_Tcp_Tx[53] = 0x00;
    Heart_Tcp_Tx[54] = 0x00;
    Heart_Tcp_Tx[55] = 0x2C;
    Heart_Tcp_Tx[58] = 0x5A;
    Heart_Tcp_Tx[59] = 0xAF;

    Capabilities_TCP_Tx[52] = 0x00;
    Capabilities_TCP_Tx[53] = 0x00;
    Capabilities_TCP_Tx[54] = 0x00;
    Capabilities_TCP_Tx[55] = 0xAC;
    Capabilities_TCP_Tx[58] = 0x6A;
    Capabilities_TCP_Tx[59] = 0x4D;

    Upload_FCBJ_Tcp_Tx[52] = 0x00;
    Upload_FCBJ_Tcp_Tx[53] = 0x00;
    Upload_FCBJ_Tcp_Tx[54] = 0x00;
    Upload_FCBJ_Tcp_Tx[55] = 0x58;
    Upload_FCBJ_Tcp_Tx[58] = 0x4C;
    Upload_FCBJ_Tcp_Tx[59] = 0xB2;

    Upload_FCHF_Tcp_Tx[52] = 0x00;
    Upload_FCHF_Tcp_Tx[53] = 0x00;
    Upload_FCHF_Tcp_Tx[54] = 0x00;
    Upload_FCHF_Tcp_Tx[55] = 0x58;
    Upload_FCHF_Tcp_Tx[58] = 0xC4;
    Upload_FCHF_Tcp_Tx[59] = 0xD0;

    Upload_Alarm_Tcp_Tx_1_1[52] = 0x00;
    Upload_Alarm_Tcp_Tx_1_1[53] = 0x00;
    Upload_Alarm_Tcp_Tx_1_1[54] = 0x00;
    Upload_Alarm_Tcp_Tx_1_1[55] = 0x6C;
    Upload_Alarm_Tcp_Tx_1_1[58] = 0x5A;
    Upload_Alarm_Tcp_Tx_1_1[59] = 0xC2;
    
    
    Upload_Alarm_Tcp_Tx_2_1[52] = 0x00;
    Upload_Alarm_Tcp_Tx_2_1[53] = 0x00;
    Upload_Alarm_Tcp_Tx_2_1[54] = 0x00;
    Upload_Alarm_Tcp_Tx_2_1[55] = 0x6C;
    Upload_Alarm_Tcp_Tx_2_1[58] = 0xD6;
    Upload_Alarm_Tcp_Tx_2_1[59] = 0x8B;
    
    
    Upload_Alarm_Tcp_Tx_3_1[52] = 0x00;
    Upload_Alarm_Tcp_Tx_3_1[53] = 0x00;
    Upload_Alarm_Tcp_Tx_3_1[54] = 0x00;
    Upload_Alarm_Tcp_Tx_3_1[55] = 0x6C;
    Upload_Alarm_Tcp_Tx_3_1[58] = 0x37;
    Upload_Alarm_Tcp_Tx_3_1[59] = 0x94;
    
    Upload_Alarm_Tcp_Tx_4_1[52] = 0x00;
    Upload_Alarm_Tcp_Tx_4_1[53] = 0x00;
    Upload_Alarm_Tcp_Tx_4_1[54] = 0x00;
    Upload_Alarm_Tcp_Tx_4_1[55] = 0x6C;
    Upload_Alarm_Tcp_Tx_4_1[58] = 0xA2;
    Upload_Alarm_Tcp_Tx_4_1[59] = 0x09;
    

    Query_Tcp_Tx_1_1_1[52] = 0x00;
    Query_Tcp_Tx_1_1_1[53] = 0x00;
    Query_Tcp_Tx_1_1_1[54] = 0x00;
    Query_Tcp_Tx_1_1_1[55] = 0x98;
    Query_Tcp_Tx_1_1_1[58] = 0x5E;
    Query_Tcp_Tx_1_1_1[59] = 0xB1;

    Query_Tcp_Tx_1_1_0[52] = 0x00;
    Query_Tcp_Tx_1_1_0[53] = 0x00;
    Query_Tcp_Tx_1_1_0[54] = 0x00;
    Query_Tcp_Tx_1_1_0[55] = 0x98;
    Query_Tcp_Tx_1_1_0[58] = 0xB8;
    Query_Tcp_Tx_1_1_0[59] = 0X39;


    Query_Tcp_Tx_1_0_0[52] = 0x00;
    Query_Tcp_Tx_1_0_0[53] = 0x00;
    Query_Tcp_Tx_1_0_0[54] = 0x00;
    Query_Tcp_Tx_1_0_0[55] = 0x98;
    Query_Tcp_Tx_1_0_0[58] = 0x3D;
    Query_Tcp_Tx_1_0_0[59] = 0xC3;
}

void Register_send(void)
{
    uint16_t i, tmp;
    tmp = strlen(Register_data_Tx);
    for (i = 0; i < 60; i++)
    {
        uart2SendByte(Tcp_header[i]);
    }
    for (i = 0; i < tmp; i++)
    {
        uart2SendByte(Register_data_Tx[i]);
    }
}

void Heart_send(void)
{
    uint16_t i;
    for (i = 0; i < 104; i++)
    {
        uart2SendByte(Heart_Tcp_Tx[i]);
    }
}

void Capabilities_send(void)
{
    uint16_t i;
    for (i = 0; i < 232; i++)
    {
        uart2SendByte(Capabilities_TCP_Tx[i]);
    }
}

void FCBJ_send(void)
{
    uint16_t i;
    for (i = 0; i < 148; i++)
    {
        uart2SendByte(Upload_FCBJ_Tcp_Tx[i]);
    }
}

void FCHF_send(void)
{
    uint16_t i;
    for (i = 0; i < 148; i++)
    {
        uart2SendByte(Upload_FCHF_Tcp_Tx[i]);
    }
}
void Upload_Alarm_send(char *p)
{
    uint16_t i;
    for (i = 0; i < 168; i++)
    {
        uart2SendByte(*p);
        p++;
    }
}

void Query_send(char *p)
{
    uint16_t i;
    for (i = 0; i < 212; i++)
    {
        uart2SendByte(*p);
        p++;
    }
}

void Communicate_Network(void)
{
    uint16_t i, tmp_length;
//    char *tmp;
    unsigned char tmp_flag;

    for (i = 0; i < 60; i++)
    {
        Deal_data_Header[i] = 0;
        Deal_data_Header[i] = gComBuf[i];
    }
    tmp_length = Deal_data_Header[54] * 256 + Deal_data_Header[55];
    for (i = 0; i < tmp_length; i++)
    {
        Deal_data[i] = 0;
        Deal_data[i] = gComBuf[i + 60];
    }
    for (i = 0; i < 300; i++)
    {
        gComBuf[i] = 0x00;
    }
    if ((strstr(Deal_data, Command_Register)) != NULL)//�ж��Ƿ��ж�Ӧ��ע�������ֶΣ�������������صĴ�����ע��ɹ�����
    {
        tmp_flag = 0x01;
    }
    else if ((strstr(Deal_data, Command_Heart)) != NULL)//���������ݽ��ջظ�
    {         
        tmp_flag = 0x02;
    }
    else if ((strstr(Deal_data, Command_Query)) != NULL)//�豸״̬�ظ�
    {
        tmp_flag = 0x03;
    }
    else if ((strstr(Deal_data, Command_Capabilities)) != NULL)//������ѯ��
    {
        tmp_flag = 0x04;
    }
    else if ((strstr(Deal_data, Command_Setuparm_BF)) != NULL)//����
    {
        tmp_flag = 0x05;
    }
    else if ((strstr(Deal_data, Command_Setuparm_CF)) != NULL)//����
    {
        tmp_flag = 0x06;
    }
    else if ((strstr(Deal_data, Command_Clearalarm)) != NULL)
    {
        tmp_flag = 0x07;
    }
    else if ((strstr(Deal_data, Command_Alarmtime)) != NULL)
    {
        tmp_flag = 0x08;
    }
    switch (tmp_flag)
    {
    case 0x01://ע��ظ�
        Register_flag = 0xff;
        tmp_flag = 0x00;
        break;
    case 0x02://�����ظ�
        Heart_flag = 0x55;
        tmp_flag = 0x00;
        break;
    case 0x03:
        Query_flag = 0xff;
        tmp_flag = 0x00;
        break;
    case 0x04:
        Capabilities_flag = 0xff;
        tmp_flag = 0x00;
        break;
    case 0x05:
        tmp_flag = 0x00;
        FQBF_flag = 0xff;
        break;
    case 0x06:
        tmp_flag = 0x00;
        FQCF_flag = 0xff;
        break;
		case 0x07:
        tmp_flag = 0x00;
        Clearalarm_flag = 0xff;
        break;
    default:
        break;
    }
//      free(tmp);
//      tmp=NULL;
}

void TaskUsartProcess(void *pdata)
{

    while (1)
    {
        OSTimeDly(5);

//���𱨾�
        if ((OutPut.FCKJ_Flag == 8) && (Fckg_flag == 0x00) ) //���𱨾���־ InPut.Fckg==0��ʾ������InPut.Fckg==1��ʾ���� InPut.Fckg == 0
        {
            FCBJ_send();
            Fckg_flag = 0xff;
        }
        //���𱨾��ָ�
        if ((OutPut.FCKJ_Flag == 0) && (Fckg_flag == 0xff))
        {
            FCHF_send();
            Fckg_flag = 0x00;
        }

//�豸״̬��ѯ�ظ�����
        
        //replace 0x55 with 0x02 replace 0xaa with 0x01
        if (Query_flag == 0xff)
        {
            if (Other_Dat.ZJStatus)
            {
                if (((Other_Dat.FQ1STATUS == 0x02) || (Other_Dat.FQ1STATUS == 0x01)) && ((Other_Dat.FQ2STATUS == 0x02) || (Other_Dat.FQ2STATUS == 0x01)))
                {
                    Query_send(Query_Tcp_Tx_11_11_11);
                }
                if (((Other_Dat.FQ1STATUS == 0x02) || (Other_Dat.FQ1STATUS == 0x01)) && (Other_Dat.FQ2STATUS == 0x00 ))
                {
                    Query_send(Query_Tcp_Tx_11_11_10);
                }
                if (( (Other_Dat.FQ1STATUS == 0x00)) && ((Other_Dat.FQ2STATUS == 0x02) || (Other_Dat.FQ2STATUS == 0x01)))
                {
                    Query_send(Query_Tcp_Tx_11_11_01);
                }
                if ((Other_Dat.FQ1STATUS == 0x00)  && (Other_Dat.FQ2STATUS == 0x00 ))
                {
                    Query_send(Query_Tcp_Tx_11_11_00);
                }
            }
            else
            {
                Query_send(Query_Tcp_Tx_11_00_00);
            }
            Heart_time_Count = 0;
            Query_flag = 0x00;
        }
//�豸�������ظ�
        if (Capabilities_flag == 0xff)
        {
            Capabilities_send();
            Capabilities_flag = 0x00;
        }
//��������
        if (FQCF_flag == 0xff)
        {
            if (bSTATUS.FQ1_RUNFLAG)
                fqSTATUS.FQ1CHEFANG = 0xAA; 
            if (bSTATUS.FQ2_RUNFLAG)
                fqSTATUS.FQ2CHEFANG = 0xAA;
            FQCF_flag = 0x00;
        }
//��������
        if (FQBF_flag == 0xff)
        {
            POWER_FIRSTON = 0;
            POWER_ONCNT = 0;
            if (bSTATUS.FQ1_RUNFLAG == 0)
                fqSTATUS.FQ1BUFANG = 0xAA; 
            if (bSTATUS.FQ2_RUNFLAG == 0)
                fqSTATUS.FQ2BUFANG = 0xAA;
            FQBF_flag = 0x00;
        }
        //����
        if(Clearalarm_flag == 0xff)
        {
            if(OutPut.Ird_Flag == 8)
            {
                OutPut.Ird_Exit_Js = 0;
                OutPut.Ird_Flag = 0;
                OutPut.FangQu1_Bj_Beep = 0;
                OutPut.FangQu2_Bj_Beep = 0;
                //OutPut.FCKJ_Flag=0;
                BEEP_OFF();
                bSTATUS.IRAlarm = 0;
            }
            if ( OutPut.FCKJ_Flag == 8)
            {
                OutPut.FCKJ_Exit_Js = 0;
                OutPut.FCKJ_Flag = 0;
                OutPut.FangQu1_Bj_Beep = 0;
                OutPut.FangQu2_Bj_Beep = 0;
                BEEP_OFF();
                bSTATUS.IRAlarm = 0;
            }
            if ((OutPut.FangQu1_Bj_Beep == 8) || (OutPut.FangQu2_Bj_Beep == 8))
            {
                OutPut.Beep_Js = 0;
                if ( OutPut.FangQu1_Bj_Beep == 8)
                    OutPut.FangQu1_Bj_Beep = 0;
                if ( OutPut.FangQu2_Bj_Beep == 8)
                    OutPut.FangQu2_Bj_Beep = 0;
                BEEP_OFF();
                bSTATUS.IRAlarm = 0;
            }
            if (( OutPut.Ird_Flag1 == 8  ) || ( OutPut.FCKJ_Flag1 == 8 )) //����1�̵��������������
            {

                if ( OutPut.Ird_Flag1 == 8  )
                    OutPut.Ird_Flag1  = 0;
                else if ( OutPut.FCKJ_Flag1 == 8 )
                    OutPut.FCKJ_Flag1 = 0;
                OutPut.Ird_RelayJs = 0;
                RELAY3_OFF();
            }
            if ( OutPut.FangQu1_Bj_Relay == 8  )
            {
                OutPut.FangQu1_Bj_Relay = 0;
                OutPut.Rellay_Js1 = 0;
                RELAY1_OFF();
            }
            if ( OutPut.FangQu2_Bj_Relay == 8  )
            {
                OutPut.FangQu2_Bj_Relay = 0;
                OutPut.Rellay_Js2 = 0;
                RELAY2_OFF();
            }
            Clearalarm_flag = 0x00;
        }
         //replace 0x55 with 0x02 replace 0xaa with 0x01
        if ((Other_Dat.FQ1STATUS == 0x02) && (Other_Dat.FQ2STATUS == 0x02) && (FQBJ_flag_1 == 0x00))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_1_11);
            FQBJ_flag_1 = 0xff;
        }
        if ((Other_Dat.FQ1STATUS == 0x02) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_2 == 0x00))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_1_10);
            FQBJ_flag_2 = 0xff;
        }
        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x02) && (FQBJ_flag_3 == 0x00))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_1_01);
            FQBJ_flag_3 = 0xff;
        }

        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_1 == 0xff))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_2_11);
            FQBJ_flag_1 = 0x00;
        }
        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_2 == 0xff))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_2_10);
            FQBJ_flag_2 = 0x00;
        }
        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_3 == 0xff))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_2_01);
            FQBJ_flag_3 = 0x00;
        }

//������������·����
        if ((Other_Dat.FQ1STATUS == 0x01) && (Other_Dat.FQ2STATUS == 0x01) && (FQBJ_flag_4 == 0x00))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_3_11);
            FQBJ_flag_4 = 0xff;
        }
        if ((Other_Dat.FQ1STATUS == 0x01) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_5 == 0x00))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_3_10);
            FQBJ_flag_5 = 0xff;
        }
        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x01) && (FQBJ_flag_6 == 0x00))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_3_01);
            FQBJ_flag_6 = 0xff;
        }

        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_4 == 0xff))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_4_11);
            FQBJ_flag_4 = 0x00;
        }
        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_5 == 0xff))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_4_10);
            FQBJ_flag_5 = 0x00;
        }
        if ((Other_Dat.FQ1STATUS == 0x00) && (Other_Dat.FQ2STATUS == 0x00) && (FQBJ_flag_6 == 0xff))
        {
            Upload_Alarm_send(Upload_Alarm_Tcp_Tx_4_01);
            FQBJ_flag_6 = 0x00;
        }
    }
}








