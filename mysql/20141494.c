#include <stdio.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "rkdehddnr1";
const char* db = "project";
MYSQL conn;
MYSQL* connection = NULL;

void send_query(char* query);
void start_conversation();

int main(void) {

	MYSQL_ROW sql_row;

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}

	else
	{
		printf("Connection Succeed\n");

		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}

		start_conversation();
			

		mysql_close(connection);

		printf("");
	}

	return 0;
}

void send_query(char* query)
{
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	int state = 0;
	int field_count,i;

	state = mysql_query(connection, query);
	
	
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
		field_count = mysql_num_fields(sql_result);

		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			for (i = 0; i < field_count; i++)
				printf("%s ", sql_row[i]);
			printf("\n");
		}
		mysql_free_result(sql_result);
	}
}

void start_conversation()
{
	int input;

	while (true)
	{
		printf("------- SELECT QUERY TYPES -------\n\n");
		printf("\t1. TYPE I\n");
		printf("\t2. TYPE II\n");
		printf("\t3. TYPE III\n");
		printf("\t4. TYPE IV\n");
		printf("\t5. TYPE V\n");
		printf("\t0. QUIT\n");
		printf("Which type of query?");
		scanf("%d", &input);

		switch (input)
		{
		case 0:
			return;
		case 1:
			printf("----- Subtypes in Type I -----\n");
			printf("\t1. TYPE I-1.\n");
			printf("\t2. TYPE I-2.\n");
			printf("\t3. TYPE I-3.\n");
			scanf("%d", input);
			switch (input)
			{
			case 1:
				send_query("a");
				break;
			case 2:
				send_query("a");
				break;
			case 3:
				send_query("a");
				break;
			default:
				printf("Wrong Input\n");
				break;
			}
			break;
		case 2:
			send_query("a");
			break;
		case 3:
			send_query("a");
			break;
		case 4:
			send_query("a");
			break;
		case 5:
			send_query("a");
			break;
			break;
		default:
			printf("Wrong Input\n");
			break;
		}

	}



}



/*


	printf("\n\n");
	printf("---- TYPE II ----\n\n");
	printf("** Find the customer who has shipped the most packages in certain year**\n");
	printf(" Which Year? : 2018\n");
	printf(" Customer Name : Kim Yongdam\n");

	return 0;

}
*/