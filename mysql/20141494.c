#include <stdio.h>
#include <string.h>
#include "mysql.h"

#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "rkdehddnr1";
const char* db = "project2";
MYSQL conn;
MYSQL* connection = NULL;

MYSQL_RES* send_query(char* query);
void start_conversation();
void type1_conversation();
void subtype1_conversation(int vehicle_id);
void type1_1_conversation(int vehicle_id);
void type1_2_conversation(int vehicle_id);
void type1_3_conversation(int vehicle_id);
void type2_conversation();
void type3_conversation();
void type4_conversation();
void type5_conversation();


char* find_truck =
" select vehicle_id,state from vehicle where vehicle_id = %d and state=\"destroyed\";";


const char* query1_1 =
"SELECT distinct package.customer_name, package.customer_address \
FROM package, tracking_info \
WHERE package.package_id = tracking_info.package_id \
and vehicle_id = %d and arrival_time is NULL;";

const char* query1_2 =
"SELECT distinct package.recipient \
FROM package, tracking_info \
WHERE package.package_id = tracking_info.package_id \
and vehicle_id = %d and arrival_time is NULL;";

const char* query1_3 =
"WITH VICTIMS(package_id) AS ( \
SELECT package.package_id \
FROM package, tracking_info \
WHERE package.package_id = tracking_info.package_id \
and vehicle_id = %d and arrival_time is NULL \
) \
select tracking_info.package_id, MAX(departure_time) \
	FROM VICTIMS, tracking_info \
where VICTIMS.package_id = tracking_info.package_id \
and tracking_info.arrival_time IS NOT NULL \
group by package_id ;";

const char* query2 =
"select customer_name, customer_address, count(*) as package_count \
from package \
where year(package.registration_time) = %d \
group by customer_name, customer_address \
order by package_count \
limit 1;";

const char* query3 =
"select customer_name, customer_address, sum(fee) as total_fee \
from package \
where year(package.registration_time) = %d \
group by customer_name, customer_address \
order by total_fee \
limit 1; ";

const char* query4 =
"SELECT package.package_id \
FROM package, tracking_info \
WHERE package.package_id = tracking_info.package_id and \
package.promised_time < tracking_info.arrival_time and \
	to_warehouse IS NULL;";

const char* query5_1 =
"select customer_name, customer_address, sum(fee) as total_fee \
from package \
where customer_name = \"%s\" and year(package.registration_time) = %d and month(package.registration_time) = %d \
group by customer_name, customer_address;";

const char* query5_2 =
"select customer_name, customer_address, type_, sum(fee) as total_fee \
from package \
where customer_name = \"%s\" and year(package.registration_time) = %d and month(package.registration_time) = %d \
group by customer_name, customer_address, type_ \
order by customer_name, customer_address, type_;";

const char* query5_3 =
"select package.customer_address, package_id, content, fee, type_, pay_contract.payment_type, timeliness \
from package, pay_contract \
where pay_contract.customer_name = package.customer_name and \
	  pay_contract.customer_address = package.customer_address and \
      pay_contract.shipper_id = package.shipper_id  and \
      package.customer_name = \"%s\" and year(package.registration_time) = %d and month(package.registration_time) = %d \
order by package.customer_address;";


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

		printf("Database Quit successfully\n");
	}

	return 0;
}

MYSQL_RES* send_query(char* query)
{
	MYSQL_RES* sql_result = NULL;
	int state = 0;
	int field_count,i;

	state = mysql_query(connection, query);
	
	
	if (state == 0)
	{
		sql_result = mysql_store_result(connection);
	}

	return sql_result;
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
			type1_conversation();

			break;
		case 2:
			type2_conversation();
			break;
		case 3:
			type3_conversation();
			break;
		case 4:
			type4_conversation();
			break;
		case 5:
			type5_conversation();
			break;
		default:
			printf("Wrong Input\n");
			break;
		}

	}
}

void type1_conversation()
{
	int input, vehicle_id;
	MYSQL_ROW sql_row = NULL;
	MYSQL_RES* sql_result = NULL;
	char buffer[200];

	while (true)
	{
		printf("----- Type I -----\n");		
		printf("Input the number of truck:");

		scanf("%d", &input);

		sprintf(buffer, find_truck, input);

		if (input == 0)
			return;

		sql_result = send_query(buffer);
		vehicle_id = input;
		// 트럭이 파괴되지 않았으면
		if (mysql_fetch_row(sql_result) == NULL)
		{
			printf("Truck %d is not destroyed.\n\n", input);
			mysql_free_result(sql_result);
			continue;
		}
		else 
		{
			mysql_free_result(sql_result);
			subtype1_conversation(vehicle_id);
		}
	}
}

void subtype1_conversation(int vehicle_id)
{
	int input;
	while (true)
	{
		printf("----- Subtypes in Type I -----\n");
		printf("\t1. TYPE I-1.\n");
		printf("\t2. TYPE I-2.\n");
		printf("\t3. TYPE I-3.\n");
		printf("Which type of query?");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			type1_1_conversation(vehicle_id);
			break;
		case 2:
			type1_2_conversation(vehicle_id);
			break;
		case 3:
			type1_3_conversation(vehicle_id);
			break;
		case 0:
			return;
		default:
			printf("Wrong Input\n");
			break;
		}
	}
}

void type1_1_conversation(int vehicle_id)
{
	char buffer[1000];
	printf("----- Type I-1 -----\n");
	printf("** Find all customers who had a package on the truck at the time of the crash. **\n");
	MYSQL_ROW sql_row = NULL;
	MYSQL_RES* sql_result = NULL;

	sprintf(buffer, query1_1, vehicle_id);

	sql_result = send_query(buffer);

	printf("Customer Name : ");

	// foreach row
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%s ,", sql_row[0]);
	}
	mysql_free_result(sql_result);
	printf("\n\n");

	return;
}


void type1_2_conversation(int vehicle_id)
{
	char buffer[1000];
	printf("----- Type I-2 -----\n");
	printf("** Find all recipients who had a package on that truck at the time of the crash.  **\n");
	MYSQL_ROW sql_row = NULL;
	MYSQL_RES* sql_result = NULL;


	sprintf(buffer, query1_2, vehicle_id);
	sql_result = send_query(buffer);

	printf("Recipients : ");

	// foreach row
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%s ,", sql_row[0]);
	}
	mysql_free_result(sql_result);
	printf("\n\n");

	return;
}

void type1_3_conversation(int vehicle_id)
{
	char buffer[1000];
	printf("----- Type I-3 -----\n");
	printf("** Find the last successful delivery by that truck prior to the crash.   **\n");
	MYSQL_ROW sql_row = NULL;
	MYSQL_RES* sql_result = NULL;

	sprintf(buffer, query1_3, vehicle_id);

	sql_result = send_query(buffer);

	printf("(pacakge_id, departure_time) : ");

	// foreach row
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("(%s, %s), ", sql_row[0], sql_row[1]);
	}
	mysql_free_result(sql_result);
	printf("\n\n");

	return;
}

void type2_conversation()
{
	int input;
	char buffer[1000];
	while (true)
	{
		MYSQL_ROW sql_row = NULL;
		MYSQL_RES* sql_result = NULL;

		printf("----- Type II -----\n");
		printf("** Find the customer who has shipped the most packages in the past certain year.  **\n");
		printf("Which year?");
		scanf("%d", &input);
		if (input == 0)
			return;

		sprintf(buffer, query2, input);

		sql_result = send_query(buffer);

		printf("(customer_name, customer_address) : ");

		// foreach row
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("(%s, %s), ", sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
		printf("\n\n");
	}

	return;
}

void type3_conversation()
{
	int input;
	char buffer[1000];
	while (true)
	{
		MYSQL_ROW sql_row = NULL;
		MYSQL_RES* sql_result = NULL;

		printf("----- Type III -----\n");
		printf("** Find the customer who has spent the most money on shipping in the past certain year.  **\n");
		printf("Which year?");
		scanf("%d", &input);
		if (input == 0)
			return;

		sprintf(buffer, query3, input);

		sql_result = send_query(buffer);

		printf("(customer_name, customer_address) : ");

		// foreach row
		while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
		{
			printf("(%s, %s), ", sql_row[0], sql_row[1]);
		}
		mysql_free_result(sql_result);
		printf("\n\n");
	}

	return;
}

void type4_conversation()
{
	printf("----- Type IV -----\n");
	printf("** Find those packages that were not delivered within the promised time. **\n");
	MYSQL_ROW sql_row = NULL;
	MYSQL_RES* sql_result = NULL;

	sql_result = send_query(query4);

	printf("package_id: ");

	// foreach row
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%s,", sql_row[0]);
	}
	mysql_free_result(sql_result);
	printf("\n\n");

	return;
}

void type5_conversation()
{
	int input;
	char buffer[1000];
	char name[20];
	int month, year;
	MYSQL_ROW sql_row = NULL;
	MYSQL_RES* sql_result = NULL;

	printf("----- Type V -----\n");
	printf("** Generate the bill for each customer for the past certain month. Consider creating several types of bills.  **\n");
	
	printf("Customer Name : ");
	scanf("%s", &name);

	printf("Which month(YYYY-MM)? ");
	scanf("%d-%d", &year, &month);

	// simple bill
	sprintf(buffer, query5_1, name, year,month);

	sql_result = send_query(buffer);

	printf("--- name, address, total_fee ---\n");
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%s, %s, %s\n", sql_row[0], sql_row[1], sql_row[2]);
	}
	mysql_free_result(sql_result);
	printf("\n\n");

	// A bill listing charges by type of service.
	sprintf(buffer, query5_2, name, year, month);

	sql_result = send_query(buffer);
	printf("--- name, address, type, total_fee ---\n");
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%s, %s, %s, %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3]);
	}
	mysql_free_result(sql_result);
	printf("\n\n");

	// An itemize billing listing each individual shipment and the charges for it. 
	sprintf(buffer, query5_3, name, year, month);

	sql_result = send_query(buffer);
	printf("--- address,package_id, content, fee, service_type, pay_type,timeliness ---\n");
	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%s, %s, %s, %s, %s, %s, %s\n", sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4], sql_row[5], sql_row[6]);
	}
	mysql_free_result(sql_result);
	printf("\n\n");

	return;

}

