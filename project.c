#include <gtk/gtk.h>
#include <string.h>

typedef struct {
    char name[50];
    char details[50];
} Customer;


typedef struct {
    char date[25];
    float value;
    char type[50];
    char name[50];
    char to_partner[50];
    int to_partner_index;
    char from_account[50];
    int from_account_index;
    char to_account[50];
    int to_account_index;
} Transaction;

typedef struct {
    char name[50];
 
    char type[20];
    float value;
    char notes[50];
} Account;




Account *accounts;
Transaction *transactions;
Customer *partners;
char *username;
char *password;

gchar *audit_labels[150];
GtkWidget *your_accounts_box , *previous_transactions_box , *previous_partners_box , *financial_report_box;
GtkWidget *main_window_global = NULL;
GtkWidget *show_editable_window_global = NULL;
GtkWidget *transactions_window_global = NULL;

int accounts_number = 0 , transaction_number = 0 ,  partners_number = 0 , logs_number = 0;

// Function to validate a date(DD/MM/YYYY) sent as a char
int valid_date(const char *a)
{
    if (strlen(a) > 10)
        return 0;
    if ((a[0] >= '4' || a[0] < '0') ||  (a[0] == '3' && a[1] >= '2') || (a[1] > '9' || a[1] < '0') || (a[0] == '0' && a[1] == '0')) // check if the day entered by the user is valid
        return 0;
    if (a[2] != '/')
        return 0;
    if ((a[3] >= '2' || a[3] < '0') || (a[3] == '1' && a[4] >= '3' ) || (a[4] > '9' || a[4] < '0') || (a[0] == '0' && a[1] == '0'))  // check if the month entered by the user is valid
        return 0;
    if (a[5] != '/')
        return 0;
    if ((a[6] > '9' || a[6] < '0') || (a[7] > '9' || a[7] < '0') || (a[8] > '9' || a[8] < '0') || (a[9] > '9' || a[9] < '0'))  // check if the year entered by the user is valid
        return 0;
    return 1;
}



int valid_amount(const char *a) // checks if the amount entered is a number
{
    for (int i = 0 ; i < strlen(a) ; i++)
        if((a[i] >'9' || a[i] < '0') && (a[i] != '.'))
            return 0;
    return 1;
}


void show_error (gchar *message) // Function to create a new dialog  with the message error
{
    GtkWidget *error = gtk_message_dialog_new(GTK_WINDOW(NULL),GTK_DIALOG_MODAL , GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,"%s",message); 
    gtk_dialog_run(GTK_DIALOG(error));
    gtk_widget_destroy(error);
}
void show_account_in_box()// Function to show all available account in the accounts management window in the right handside of the window
{

    gtk_container_foreach(GTK_CONTAINER(your_accounts_box), (GtkCallback)gtk_widget_destroy, NULL);// We destryoy the instances that were in the accounts box so we might have new others
    GtkWidget *label1 = gtk_label_new("------------Your accounts------------ ");// The label that is on top side of the box that has the accounts
        
        //Apply CSS customization
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 24px;  font-weight: bold;}", -1, NULL);

        
        GtkStyleContext *context = gtk_widget_get_style_context(label1); // Apply the customization to the label
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_box_pack_start(GTK_BOX(your_accounts_box), label1, FALSE, FALSE, 0);//Add the label in the top side of the box
  
    for(int i = 1; i <= accounts_number ; i++)
       { 
        GtkWidget *label = gtk_label_new(g_strdup_printf("Account Type: %s\nName: %s\nValue: %.2f lei \nNotes: %s\n",
                                                        accounts[i].type,
                                                        accounts[i].name,
                                                        accounts[i].value,
                                                        accounts[i].notes));
                                                          GdkColor color; // We create a new label that has the current accounts info
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

        GtkStyleContext *context = gtk_widget_get_style_context(label); // Apply the customization to the label
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        gtk_box_pack_start(GTK_BOX(your_accounts_box), label, FALSE, FALSE, 0); // Add the label with the account info to the box
       }
       gtk_widget_show_all(your_accounts_box);//Show the box
}

void show_partners_in_box()// Function to show all available partner in the partners management window in the right handside of the window
{
    gtk_container_foreach(GTK_CONTAINER(previous_partners_box), (GtkCallback)gtk_widget_destroy, NULL);// We destryoy the instances that were in the partners box as we might have new others
    GtkWidget *label1 = gtk_label_new("------------Your partners------------ ");// The label that is on top side of the box that has the partners
    gtk_box_pack_start(GTK_BOX(previous_partners_box), label1, FALSE, FALSE, 0);

        //Apply CSS customization
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 24px;  font-weight: bold;}", -1, NULL);

  
        GtkStyleContext *context = gtk_widget_get_style_context(label1);//// Apply the customization to the label
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

  
    for(int i = 1; i <= partners_number ; i++)
       { 
        GtkWidget *label = gtk_label_new(g_strdup_printf("Name: %s\nDetails: %s\n",
                                                        partners[i].name,
                                                        partners[i].details));//Create a label with the current partners info
        
        GtkCssProvider *provider = gtk_css_provider_new(); 
        gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

        
        GtkStyleContext *context = gtk_widget_get_style_context(label);//Add the customization to the label
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_box_pack_start(GTK_BOX(previous_partners_box), label, FALSE, FALSE, 0);//Add the label with the partner info to the box
       }
       gtk_widget_show_all(previous_partners_box);//Show the box with all the partners
}

void show_transactions_in_box()// Function to show all available transactions in the transactions management window in the right handside of the window
{
    gtk_container_foreach(GTK_CONTAINER(previous_transactions_box), (GtkCallback)gtk_widget_destroy, NULL);// We destryoy the instances that were in the partners box as we might have new others
    GtkWidget *label1 = gtk_label_new("------------Previous transactions------------ ");// The label that is on top side of the box that has the transactions
    gtk_box_pack_start(GTK_BOX(previous_transactions_box), label1, FALSE, FALSE, 0);//Add the label to the box

     //Apply CSS customization
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 24px;  font-weight: bold;}", -1, NULL);

      
    GtkStyleContext *context = gtk_widget_get_style_context(label1);// Apply the customization to the label
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  
    for(int i = 1; i <= transaction_number ; i++)
       {
            //Depending on the transaction type we create new labels with the transactions info
            if (strcmp(transactions[i].type , "Income") == 0)
            {
                GtkWidget *label = gtk_label_new(g_strdup_printf("Transaction Type: %s\nName: %s\nDate: %s\nFrom: %s\nAccount: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].to_partner,
                                                                transactions[i].to_account,
                                                                transactions[i].value 
                                                                ));//Create a label with the current income info

                GtkCssProvider *provider = gtk_css_provider_new();
                gtk_css_provider_load_from_data(provider, "label { color: #00FF00;font-size: 18px;  font-weight: bold;  }", -1, NULL);

                GtkStyleContext *context = gtk_widget_get_style_context(label);// Apply the customization to the label
                gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

                gtk_box_pack_start(GTK_BOX(previous_transactions_box), label, FALSE, FALSE, 0);  //Add the label with the income info to the box                                              
            }
            if (strcmp(transactions[i].type , "Expense") == 0)
            {
                GtkWidget *label = gtk_label_new(g_strdup_printf("Transaction Type: %s\nName: %s\nDate: %s\nTo: %s\nAccount: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].to_partner,
                                                                transactions[i].to_account,
                                                                transactions[i].value 
                                                                ));//Create a label with the current expense info

                GtkCssProvider *provider = gtk_css_provider_new();
                gtk_css_provider_load_from_data(provider, "label { color: #800000;font-size: 18px;  font-weight: bold;  }", -1, NULL);

                
                GtkStyleContext *context = gtk_widget_get_style_context(label);// Apply the customization to the label
                gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
                
                gtk_box_pack_start(GTK_BOX(previous_transactions_box), label, FALSE, FALSE, 0);//Add the label with the expense info to the box
            }
            if (strcmp(transactions[i].type , "Transfer") == 0)
            {
                GtkWidget *label = gtk_label_new(g_strdup_printf("Transaction Type: %s\nName: %s\nDate: %s\nFrom: %s\nTo: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].from_account,
                                                                transactions[i].to_account,
                                                                transactions[i].value 
                                                                ));//Create a label with the current transfer info

                GtkCssProvider *provider = gtk_css_provider_new();
                gtk_css_provider_load_from_data(provider, "label { color: yellow;font-size: 18px;  font-weight: bold;  }", -1, NULL);

                GtkStyleContext *context = gtk_widget_get_style_context(label);
                gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
                gtk_box_pack_start(GTK_BOX(previous_transactions_box), label, FALSE, FALSE, 0);//Add the label with the transfer info to the box
            }
            
            
       }
       gtk_widget_show_all(previous_transactions_box);//show the box with the transactions
}

void back(GtkWidget *back_button , gpointer data)//We call the back function when we are in a specific window and we want to go back to the main window
{
    gtk_widget_destroy(gtk_widget_get_toplevel(back_button));//Destroy the window that has the button level
     if (main_window_global) {
        gtk_widget_show(main_window_global);//Then show the main window of the application
    }
}
void create_account_dynamic(GtkWidget *submit_button, gpointer data) //Function to add into structs the user input about an account 
{
    GtkWidget **entries = (GtkWidget **)data;//pointer to the array that has the user inputs

    const gchar *type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[0]));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *notes = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    
     if (type == NULL || strlen(name) == 0 || strlen(value_str) == 0 || strlen(notes) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{
        accounts_number++;//Increase the accounts number
        //Copy all the information into the struct
        g_strlcpy(accounts[accounts_number].type, type, sizeof(accounts[accounts_number].type));
        g_strlcpy(accounts[accounts_number].name, name, sizeof(accounts[accounts_number].name));
        g_strlcpy(accounts[accounts_number].notes, notes, sizeof(accounts[accounts_number].notes));
        accounts[accounts_number].value = atof(value_str);//Function to convert from string to a floate

        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("New account created! | Account Type: %s | Name: %s | Value: %.2f | Notes: %s",
                                                        accounts[accounts_number].type,
                                                        accounts[accounts_number].name,
                                                        accounts[accounts_number].value,
                                                        accounts[accounts_number].notes);//We update the audit log with the operation done
    }
    show_account_in_box();//Show again the box in the account management window because we added a new account

    g_free((gchar *)type);
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(submit_button));//Close the window where user created a new account
}



void create_account(GtkWidget *create_account_button, gpointer data) //Window where user is required the enter informations about an account he wants to add
{

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);//Create a new vertical box 
    gtk_container_add(GTK_CONTAINER(main_window), vbox);//Add the box to the window

    GtkWidget *grid = gtk_grid_new();//Create a new grid so we can add
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);//Add the grid to the box. In the grid we will have all the entry cases and labels neccesary for user to create a new account 

    GtkWidget *type_label = gtk_label_new("Type:");
    gtk_grid_attach(GTK_GRID(grid), type_label, 0, 0, 1, 1);//Add the label to the 0 column, 0 row of the grid
    GtkWidget **entries = (GtkWidget **)g_malloc(4 * sizeof(GtkWidget *));//Declare a new array where we will store the users input and we'll send it further to store the informations about the account

    entries[0] = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Savings");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Checking");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Credit Card");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Investment");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Wallet");
    gtk_grid_attach(GTK_GRID(grid) , entries[0] , 1 , 0 , 1 , 1);//Add the combo box to the  column 0, row 1 of the grid


    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);//Add the label to the column 0, row 1 of the grid

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter the account's name");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);//Add the entry to the column 1, row 1 of the grid

    GtkWidget *value_label = gtk_label_new("Value:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 2, 1, 1);//Add the label to the column 0 , row 2 of the grid

    entries[2] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[2]), "Enter the account's value");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[2]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[2], 1, 2, 1, 1);//Add the entry to the column 1, row 2 of the grid

    GtkWidget *notes_label = gtk_label_new("Notes:");
    gtk_grid_attach(GTK_GRID(grid), notes_label, 0, 3, 1, 1);//Add the label to the column 0 , row 3 of the grid

    entries[3] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[3]), "Additional notes");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[3]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[3], 1, 3, 1, 1);//Add the entry to the column 2, row 2 of the grid


    GtkWidget *submit_button = gtk_button_new_with_label("Submit");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(create_account_dynamic), entries);//Create a button that when it is pressed it call the function create_acount_dynamic and send the entries parameters necesarry to create a new account

    gtk_box_pack_start(GTK_BOX(vbox), submit_button, FALSE, FALSE, 0);//Add the button to the box

    gtk_widget_show_all(main_window);//Show the window
}

void edit_account_dynamic(GtkWidget *edit_account_button, gpointer data) //Function to modify the the struct that has the account that user want to modify
{
    GtkWidget **entries = (GtkWidget **)data;//pointer to the array that has the user inputs

    const gchar *type = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[0]));
    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *notes = gtk_entry_get_text(GTK_ENTRY(entries[3]));
    int *accounts_number_pointer = (int *)entries[4];
    int accounts_number = *accounts_number_pointer;//the index of the account the user wants to modify


    if (type == NULL || strlen(name) == 0 || strlen(value_str) == 0 || strlen(notes) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{
        char initial_name[25];
        //Copy all the information into the struct
        g_strlcpy(initial_name, accounts[accounts_number].name, sizeof(initial_name));
        g_strlcpy(accounts[accounts_number].type, type, sizeof(accounts[accounts_number].type));
        g_strlcpy(accounts[accounts_number].name, name, sizeof(accounts[accounts_number].name));
        g_strlcpy(accounts[accounts_number].notes, notes, sizeof(accounts[accounts_number].notes));
        accounts[accounts_number].value = atof(value_str);//Function to convert string to float
        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Edited '%s' account! | Account Type: %s | Name: %s | Value: %.2f | Notes: %s",
                                                        initial_name,
                                                        accounts[accounts_number].type,
                                                        accounts[accounts_number].name,
                                                        accounts[accounts_number].value,
                                                        accounts[accounts_number].notes);//Add the last operation to the audit log array
    }
   
    show_account_in_box();//Show again the accounts because the list was modified

    g_free((gchar *)type);
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(edit_account_button));//Close the window where user edited an account
     if (show_editable_window_global) {
        gtk_widget_destroy(show_editable_window_global);//Close the window where it shows all the available accounts
    }
}


void edit_account(GtkWidget *account_button, gpointer data)//Window where user is required to modify the informations about the account he selected
{
    int accounts_index = GPOINTER_TO_INT(data);

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);//Add the grid to the box. In the grid we will have all the entry cases and labels neccesary for user to create a new account 

    GtkWidget *type_label = gtk_label_new("Type:");
    gtk_grid_attach(GTK_GRID(grid), type_label, 0, 0, 1, 1);//Add the label to the column 0 , row 0 of the grid
    GtkWidget **entries = (GtkWidget **)g_malloc(5 * sizeof(GtkWidget *));//Declare a new array where we will store the users input and we'll send it further to store the informations about the account

    entries[0] = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Savings");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Checking");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Credit Card");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Investment");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[0]), "Wallet");
    gtk_grid_attach(GTK_GRID(grid) , entries[0] , 1 , 0 , 1 , 1);//Add the label to the  column 1, row 0 of the grid
    
    const char* type = accounts[accounts_index].type;
    int index;
    index = (strcmp(type, "Savings") == 0) ? 0 :
        (strcmp(type, "Checking") == 0) ? 1 :
        (strcmp(type, "Credit Card") == 0) ? 2 :
        (strcmp(type, "Investment") == 0) ? 3 :
        (strcmp(type, "Wallet") == 0) ? 4 :
        -1;
    gtk_combo_box_set_active(GTK_COMBO_BOX(entries[0]), index);

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 1, 1, 1);//Add the label to the column 0 , row 1 of the grid

    entries[1] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_entry_set_text(GTK_ENTRY(entries[1]), accounts[accounts_index].name);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);//Add the entry to the column 1 , row 1 of the grid

    GtkWidget *value_label = gtk_label_new("Value:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 2, 1, 1);//Add the label to the column 0 , row 2 of the grid

    entries[2] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[2]), 30);
    const char* value_str = g_strdup_printf("%f", accounts[accounts_index].value);
    gtk_entry_set_text(GTK_ENTRY(entries[2]), value_str);
    gtk_grid_attach(GTK_GRID(grid), entries[2], 1, 2, 1, 1);//Add the entry to the column 1 , row 2 of the grid

    GtkWidget *notes_label = gtk_label_new("Notes:");
    gtk_grid_attach(GTK_GRID(grid), notes_label, 0, 3, 1, 1);//Add the label to the column 0 , row 3 of the grid

    entries[3] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[3]), 30);
    gtk_entry_set_text(GTK_ENTRY(entries[3]), accounts[accounts_index].notes);
    gtk_grid_attach(GTK_GRID(grid), entries[3], 1, 3, 1, 1);//Add the entry to the column 0 , row 0 of the grid

    int *accounts_index_gtk = (int *)g_malloc(sizeof(int));
    *accounts_index_gtk = accounts_index; 
    entries[4] = (GtkWidget *)accounts_index_gtk;    

    GtkWidget *edit_account_button = gtk_button_new_with_label("Edit account");
    g_signal_connect(edit_account_button, "clicked", G_CALLBACK(edit_account_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), edit_account_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}


void show_edittable_accounts(GtkWidget *edit_account_button, gpointer data)//Function to show all the available accounts so the user can select one to modify
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management application");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    for (int i = 1; i <= accounts_number; ++i) {
        gchar *button_text = g_strdup_printf("Account Type: %s\nName: %s\nValue: %.2f lei\nNotes: %s\n",
                                             accounts[i].type,
                                             accounts[i].name,
                                             accounts[i].value,
                                             accounts[i].notes);

        GtkWidget *account_button = gtk_button_new_with_label(button_text);//Create buttons with the accounts info
        g_signal_connect(account_button, "clicked", G_CALLBACK(edit_account), GINT_TO_POINTER(i));//Connect the buttons with the edit account function
        g_free(button_text);

        gtk_box_pack_start(GTK_BOX(vbox), account_button, TRUE, TRUE, 0);
    }
    show_editable_window_global = main_window;
    gtk_widget_show_all(main_window);
}

void delete_account_dynamic(int index)//Function do delete information about an account from the struct
{
    logs_number++;
    audit_labels[logs_number] = g_strdup_printf("Deleted '%s' account!",
                                                        accounts[index].name);//We add the operation done to the audit log
    for (int i = index ; i <= accounts_number ; i++)
        accounts[i] = accounts[i + 1];
    accounts_number--;//We decrement the accounts number
    show_account_in_box();//We show again all the accounts
    
    if (show_editable_window_global) {
        gtk_widget_destroy(show_editable_window_global);//We destroy the window with all available accounts
    }
}

void delete_account(GtkWidget *delete_account_button, gpointer data)//Function which opens a new dialog and asks the user if he wants to delete this account
{   
    int accounts_index = GPOINTER_TO_INT(data);
    GtkWidget *dialog;
    GtkResponseType response;

    dialog = gtk_message_dialog_new(GTK_WINDOW(main_window_global),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "Are you sure you want to delete this account?");

   
    gtk_window_set_title(GTK_WINDOW(dialog), "Delete Account");

   
    response = gtk_dialog_run(GTK_DIALOG(dialog));


    if (response == GTK_RESPONSE_YES) {
        delete_account_dynamic(accounts_index);//If the user press Yes then we call the function to delete the account
    }

   
    gtk_widget_destroy(dialog);
}

void show_deletable_accounts(GtkWidget *account_button, gpointer data)//Function to show all available accounts so the user can check the account he wants to delete
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management application");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    for (int i = 1; i <= accounts_number; ++i) {
        gchar *button_text = g_strdup_printf("Account Type: %s\nName: %s\nValue: %.2f lei\nNotes: %s\n",
                                             accounts[i].type,
                                             accounts[i].name,
                                             accounts[i].value,
                                             accounts[i].notes);

        GtkWidget *account_button = gtk_button_new_with_label(button_text);//Create a new button with the accounts info
        g_signal_connect(account_button, "clicked", G_CALLBACK(delete_account), GINT_TO_POINTER(i));//Connect the buttons to the delete_account function
        g_free(button_text);

        gtk_box_pack_start(GTK_BOX(vbox), account_button, TRUE, TRUE, 0);// Add the buttons to the box
    }
    show_editable_window_global = main_window;
    gtk_widget_show_all(main_window);
}
void account_management(GtkWidget *account_management_button , gpointer data)//Function to create the window where we have all the buttons that deal with accounts
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window) , "Fincancial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window) , 100);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1920, 1080);
    //g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "window { background-image: url('bkg.jpg'); }", -1, NULL);

    // Apply the CSS provider to the window
    GtkStyleContext *context = gtk_widget_get_style_context(main_window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 200); // 200 is the spacing between children
    gtk_container_add(GTK_CONTAINER(main_window), hbox);

    // Create the left box
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), left_box, FALSE, FALSE, 0);

    
     GtkWidget *image = gtk_image_new_from_file("main_bank2.jpg");
    if (image != NULL) {
        gtk_box_pack_start(GTK_BOX(left_box), image, FALSE, FALSE, 0);
    } else {
        g_print("Error loading the image.\n");
    }  

    // Create the first text label
    GtkWidget *label1 = gtk_label_new("Financial Management Application");
    const gchar *css = "label { font-size: 30px; color:yellow; font-weight: bold;}";
    GtkCssProvider *provider2 = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider2, css, -1, NULL);
    GtkStyleContext *context2 = gtk_widget_get_style_context(label1);
    gtk_style_context_add_provider(context2, GTK_STYLE_PROVIDER(provider2), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider2);
    gtk_box_pack_start(GTK_BOX(left_box), label1, FALSE, FALSE, 0);

    // Create the second text label
    GtkWidget *label2 = gtk_label_new(g_strdup_printf("Welcome %s" , username));
    css = "label { font-size: 24px; font-style: italic; color: yellow; font-weight: bold; }";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label2);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label2, FALSE, FALSE, 0);

    GtkWidget *middle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), middle_box, FALSE, FALSE, 0);

    //We create a scrolled window in case we have multiple accounts and we have to scroll through them
    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(hbox), scrolledWindow, TRUE, TRUE, 0);

    your_accounts_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), your_accounts_box);

    //We create the buttons that deal with accounts and connect them to the corresponding functions

    GtkWidget *create_account_button = gtk_button_new_with_label("Create an account");
    g_signal_connect(create_account_button, "clicked", G_CALLBACK(create_account), NULL);
    GtkWidget *edit_account_button = gtk_button_new_with_label("Edit an account");
    g_signal_connect(edit_account_button, "clicked", G_CALLBACK(show_edittable_accounts), NULL);
    GtkWidget *delete_account_button = gtk_button_new_with_label("Delete an account");
    g_signal_connect(delete_account_button, "clicked", G_CALLBACK(show_deletable_accounts), NULL);
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    g_signal_connect(back_button, "clicked", G_CALLBACK(back), NULL);

    //We add the buttons to the box

    gtk_box_pack_start(GTK_BOX(middle_box), create_account_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), edit_account_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), delete_account_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), back_button, FALSE, FALSE, 0);


    show_account_in_box();
    gtk_widget_hide(main_window_global);
    gtk_widget_show_all(main_window);
}

void add_income_dynamic(GtkWidget *submit_button, gpointer data) //Function to add into structs the user input about a transaction of type income
{
    GtkWidget **entries = (GtkWidget **)data;//pointer to the array that has the user inputs

    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *date = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *to = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[2]));
    gint to_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[2]));
    const gchar *account = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
    gint account_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[3]));
    const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[4]));
    
    if (strlen(name) == 0 || strlen(date) == 0 || to == NULL || account == NULL ||strlen(value_str) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if (!valid_date(date)) {//verify if the date is valid
        show_error("Invalid date!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{

        //Copy the informations into the struct

        transaction_number++;
        g_strlcpy(transactions[transaction_number].type , "Income" , sizeof(transactions[transaction_number].type));
        g_strlcpy(transactions[transaction_number].name , name , sizeof(transactions[transaction_number].name));
        g_strlcpy(transactions[transaction_number].date, date, sizeof(transactions[transaction_number].date));
        g_strlcpy(transactions[transaction_number].to_partner, to, sizeof(transactions[transaction_number].to_partner));
        transactions[transaction_number].to_partner_index = to_index;
        g_strlcpy(transactions[transaction_number].to_account, account, sizeof(transactions[transaction_number].to_account));
        transactions[transaction_number].to_account_index = account_index;
        g_strlcpy(transactions[transaction_number].from_account, "-1", sizeof(transactions[transaction_number].from_account));
        transactions[transaction_number].from_account_index = -1;
        transactions[transaction_number].value = atof(value_str);
        accounts[account_index + 1].value += transactions[transaction_number].value;

        logs_number++;
    audit_labels[logs_number] = g_strdup_printf("New transaction recorded | Type: %s | Name: %s | Date: %s | From: %s | Account: %s | Value: %.2f",
                                                        transactions[transaction_number].type,
                                                        transactions[transaction_number].name,
                                                        transactions[transaction_number].date,
                                                        transactions[transaction_number].to_partner,
                                                        transactions[transaction_number].to_account,
                                                        transactions[transaction_number].value);
    }

    show_transactions_in_box();

    g_free((gchar *)to);
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(submit_button));
    if (transactions_window_global) {
        gtk_widget_destroy(transactions_window_global);
    }
}

void add_income(GtkWidget *add_income_button, gpointer data) {

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget **entries = (GtkWidget **)g_malloc(6 * sizeof(GtkWidget *));//Create an array to store the users input

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter the transaction's name");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *date_label = gtk_label_new("Date(DD/MM/YYYY):");
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter the transaction's date");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    GtkWidget *to_label = gtk_label_new("From:");
    gtk_grid_attach(GTK_GRID(grid), to_label, 0, 2, 1, 1);

    entries[2] = gtk_combo_box_text_new();
    for (int i = 1 ; i <= partners_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), partners[i].name);
    }
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), "Others");
    gtk_grid_attach(GTK_GRID(grid) , entries[2] , 1 , 2 , 1 , 1);

    GtkWidget *account_label = gtk_label_new("Account:");
    gtk_grid_attach(GTK_GRID(grid), account_label, 0, 3, 1, 1);

    entries[3] = gtk_combo_box_text_new();
    for (int i = 1 ; i <= accounts_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), accounts[i].name);
    }
    gtk_grid_attach(GTK_GRID(grid), entries[3], 1, 3, 1, 1);

    GtkWidget *value_label = gtk_label_new("Value:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 4, 1, 1);

    entries[4] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[4]), "Enter the value");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[4]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[4], 1, 4 , 1, 1);

    GtkWidget *submit_button = gtk_button_new_with_label("Submit");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(add_income_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), submit_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}

void add_expense_dynamic(GtkWidget *submit_button, gpointer data) //Function to add into structs the user input about a transaction of type expense
{
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *date = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *to = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[2]));
    gint to_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[2]));
    const gchar *account = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
    gint account_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[3]));
    const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[4]));
    

    if (strlen(name) == 0 || strlen(date) == 0 || to == NULL || account == NULL ||strlen(value_str) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if (!valid_date(date)) {//verify if the date is valid
        show_error("Invalid date!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{
        transaction_number++;
        g_strlcpy(transactions[transaction_number].type , "Expense" , sizeof(transactions[transaction_number].type));
        g_strlcpy(transactions[transaction_number].name , name , sizeof(transactions[transaction_number].name));
        g_strlcpy(transactions[transaction_number].date, date, sizeof(transactions[transaction_number].date));
        g_strlcpy(transactions[transaction_number].to_partner, to, sizeof(transactions[transaction_number].to_partner));
        transactions[transaction_number].to_partner_index = to_index;
        g_strlcpy(transactions[transaction_number].to_account, account, sizeof(transactions[transaction_number].to_account));
        transactions[transaction_number].to_account_index = account_index;
        g_strlcpy(transactions[transaction_number].from_account, "-1", sizeof(transactions[transaction_number].from_account));
        transactions[transaction_number].from_account_index = -1;
        transactions[transaction_number].value = atof(value_str);
        accounts[account_index + 1].value -=  transactions[transaction_number].value;

         logs_number++;
    audit_labels[logs_number] = g_strdup_printf("New transaction recorded | Type: %s | Name: %s | Date: %s | To: %s | Account: %s | Value: %.2f",
                                                        transactions[transaction_number].type,
                                                        transactions[transaction_number].name,
                                                        transactions[transaction_number].date,
                                                        transactions[transaction_number].to_partner,
                                                        transactions[transaction_number].to_account,
                                                        transactions[transaction_number].value);
    }
    show_transactions_in_box();

    g_free((gchar *)to);
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(submit_button));
     if (transactions_window_global) {
        gtk_widget_destroy(transactions_window_global);
    }
}

void add_expense(GtkWidget *add_expense_button, gpointer data) {

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget **entries = (GtkWidget **)g_malloc(6 * sizeof(GtkWidget *));//Pointer to the array that has the user inputs

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter the transaction's name");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *date_label = gtk_label_new("Date(DD/MM/YYYY):");
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter the transaction's date");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    GtkWidget *to_label = gtk_label_new("To:");
    gtk_grid_attach(GTK_GRID(grid), to_label, 0, 2, 1, 1);

    entries[2] = gtk_combo_box_text_new();
            for (int i = 1 ; i <= partners_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), partners[i].name);
    }
   
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), "Others");
    gtk_grid_attach(GTK_GRID(grid) , entries[2] , 1 , 2 , 1 , 1);

    GtkWidget *account_label = gtk_label_new("Account:");
    gtk_grid_attach(GTK_GRID(grid), account_label, 0, 3, 1, 1);

    entries[3] = gtk_combo_box_text_new();
    for (int i = 1 ; i <= accounts_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), accounts[i].name);
    }
    gtk_grid_attach(GTK_GRID(grid), entries[3], 1, 3, 1, 1);

    GtkWidget *value_label = gtk_label_new("Value:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 4, 1, 1);

    entries[4] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[4]), "Enter the value");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[4]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[4], 1, 4 , 1, 1);

    GtkWidget *submit_button = gtk_button_new_with_label("Submit");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(add_expense_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), submit_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}

void add_transfer_dynamic(GtkWidget *submit_button, gpointer data) //Function to add into structs the user input about a transaction of type transfer
{
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *date = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *from = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[2]));
    gint from_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[2]));
    
    const gchar *to = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
    gint to_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[3]));
    const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[4]));
    

    if (strlen(name) == 0 || strlen(date) == 0 || from == NULL || to == NULL ||strlen(value_str) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if (!valid_date(date)) {//verify if the date is valid
        show_error("Invalid date!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{

        //Copy the information into the structs

        transaction_number++;
        g_strlcpy(transactions[transaction_number].type , "Transfer" , sizeof(transactions[transaction_number].type));
        g_strlcpy(transactions[transaction_number].name , name , sizeof(transactions[transaction_number].name));
        g_strlcpy(transactions[transaction_number].date, date, sizeof(transactions[transaction_number].date));
        g_strlcpy(transactions[transaction_number].to_account, to, sizeof(transactions[transaction_number].to_account));
        transactions[transaction_number].to_account_index = to_index;
        g_strlcpy(transactions[transaction_number].from_account, from, sizeof(transactions[transaction_number].from_account));
        transactions[transaction_number].from_account_index = from_index;
        g_strlcpy(transactions[transaction_number].to_partner, "-1", sizeof(transactions[transaction_number].to_partner));
        transactions[transaction_number].to_partner_index = -1;
        transactions[transaction_number].value = atof(value_str);
        accounts[from_index + 1].value -= transactions[transaction_number].value;
        accounts[to_index + 1].value += transactions[transaction_number].value;

        logs_number++;
    audit_labels[logs_number] = g_strdup_printf("New transaction recorded | Type: %s | Name: %s | Date: %s | To Account: %s | From Account: %s | Value: %.2f",
                                                        transactions[transaction_number].type,
                                                        transactions[transaction_number].name,
                                                        transactions[transaction_number].date,
                                                        transactions[transaction_number].to_account,
                                                        transactions[transaction_number].from_account,
                                                        transactions[transaction_number].value);//Add the last operation done into the audit array
    }
    
    show_transactions_in_box();

    g_free((gchar *)to);
    g_free((gchar *)from);
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(submit_button));
     if (transactions_window_global) {
        gtk_widget_destroy(transactions_window_global);
    }
}

void add_transfer(GtkWidget *add_transfer_button, gpointer data) {

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget **entries = (GtkWidget **)g_malloc(6 * sizeof(GtkWidget *));//Create a array to store the users input

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter the transaction's name");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *date_label = gtk_label_new("Date(DD/MM/YYYY):");
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter the transaction's date");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    GtkWidget *from_account_label = gtk_label_new("From account:");
    gtk_grid_attach(GTK_GRID(grid), from_account_label, 0, 2, 1, 1);

    entries[2] = gtk_combo_box_text_new();
    for (int i = 1 ; i <= accounts_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), accounts[i].name);
    }
    gtk_grid_attach(GTK_GRID(grid) , entries[2] , 1 , 2 , 1 , 1);

    GtkWidget *to_account_label = gtk_label_new("To account:");
    gtk_grid_attach(GTK_GRID(grid), to_account_label, 0, 3, 1, 1);

    entries[3] = gtk_combo_box_text_new();
    for (int i = 1 ; i <= accounts_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), accounts[i].name);
    }
    gtk_grid_attach(GTK_GRID(grid), entries[3], 1, 3, 1, 1);

    GtkWidget *value_label = gtk_label_new("Value:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 4, 1, 1);

    entries[4] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[4]), "Enter the value");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[4]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[4], 1, 4 , 1, 1);

    GtkWidget *submit_button = gtk_button_new_with_label("Submit");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(add_transfer_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), submit_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}

void add_transaction(GtkWidget *add_transaction_button , gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    transactions_window_global = main_window;
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 300, 200);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *add_income_button = gtk_button_new_with_label("Income");
    GtkWidget *add_expense_button = gtk_button_new_with_label("Expense");
    GtkWidget *add_transfer_button = gtk_button_new_with_label("Transfer");

    gtk_box_pack_start(GTK_BOX(vbox), add_income_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), add_expense_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), add_transfer_button, TRUE, TRUE, 0);

    g_signal_connect(add_income_button, "clicked", G_CALLBACK(add_income), NULL);
    g_signal_connect(add_expense_button, "clicked", G_CALLBACK(add_expense), NULL);
    g_signal_connect(add_transfer_button, "clicked", G_CALLBACK(add_transfer), NULL);

   gtk_widget_show_all(main_window);
}

void edit_transaction_dynamic(GtkWidget *edit_transaction_button, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    int *transaction_index_pointer = (int *)entries[5];
    int transaction_index = *transaction_index_pointer;

    if (strcmp(transactions[transaction_index].type , "Income") == 0)
    {
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
        const gchar *date = gtk_entry_get_text(GTK_ENTRY(entries[1]));
        const gchar *to = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[2]));
        gint to_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[2]));
        const gchar *account = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
        gint account_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[3]));
        const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[4]));

        if (strlen(name) == 0 || strlen(date) == 0 || to == NULL || account == NULL ||strlen(value_str) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if (!valid_date(date)) {//verify if the date is valid
        show_error("Invalid date!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{
        accounts[account_index + 1].value -= transactions[transaction_index].value;

        char initial_name[25];
        g_strlcpy(initial_name , transactions[transaction_index].name , sizeof(initial_name));

        g_strlcpy(transactions[transaction_index].name , name , sizeof(transactions[transaction_index].name));
        g_strlcpy(transactions[transaction_index].date, date, sizeof(transactions[transaction_index].date));
        g_strlcpy(transactions[transaction_index].to_partner, to, sizeof(transactions[transaction_index].to_partner));
        transactions[transaction_index].to_partner_index = to_index;
        g_strlcpy(transactions[transaction_index].to_account, account, sizeof(transactions[transaction_index].to_account));
        transactions[transaction_index].to_account_index = account_index;
        g_strlcpy(transactions[transaction_index].from_account, "-1", sizeof(transactions[transaction_index].from_account));
        transactions[transaction_index].from_account_index = -1;
        transactions[transaction_index].value = atof(value_str);
        accounts[account_index + 1].value += transactions[transaction_index].value;

        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Edited '%s' transaction | Type: %s | Name: %s | Date: %s | From: %s | Account: %s | Value: %.2f",
                                                        initial_name,
                                                        transactions[transaction_number].type,
                                                        transactions[transaction_number].name,
                                                        transactions[transaction_number].date,
                                                        transactions[transaction_number].to_partner,
                                                        transactions[transaction_number].to_account,
                                                        transactions[transaction_number].value);
    }
    }
    if (strcmp(transactions[transaction_index].type , "Expense") == 0)
    {
        const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
        const gchar *date = gtk_entry_get_text(GTK_ENTRY(entries[1]));
        const gchar *to = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[2]));
        gint to_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[2]));
        const gchar *account = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
        gint account_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[3]));
        const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[4]));

        if (strlen(name) == 0 || strlen(date) == 0 || to == NULL || account == NULL ||strlen(value_str) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if (!valid_date(date)) {//verify if the date is valid
        show_error("Invalid date!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{
        accounts[account_index + 1].value += transactions[transaction_index].value;
        
        char initial_name[25];
        g_strlcpy(initial_name , transactions[transaction_index].name , sizeof(initial_name));

        g_strlcpy(transactions[transaction_index].name , name , sizeof(transactions[transaction_index].name));
        g_strlcpy(transactions[transaction_index].date, date, sizeof(transactions[transaction_index].date));
        g_strlcpy(transactions[transaction_index].to_partner, to, sizeof(transactions[transaction_index].to_partner));
        transactions[transaction_index].to_partner_index = to_index;
        g_strlcpy(transactions[transaction_index].to_account, account, sizeof(transactions[transaction_index].to_account));
        transactions[transaction_index].to_account_index = account_index;
        g_strlcpy(transactions[transaction_index].from_account, "-1", sizeof(transactions[transaction_index].from_account));
        transactions[transaction_index].from_account_index = -1;
        transactions[transaction_index].value = atof(value_str);
        accounts[account_index + 1].value -= transactions[transaction_index].value;

        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Edited '%s' transaction | Type: %s | Name: %s | Date: %s | To: %s | Account: %s | Value: %.2f",
                                                        initial_name,
                                                        transactions[transaction_number].type,
                                                        transactions[transaction_number].name,
                                                        transactions[transaction_number].date,
                                                        transactions[transaction_number].to_partner,
                                                        transactions[transaction_number].to_account,
                                                        transactions[transaction_number].value);
    }
    }
    if (strcmp(transactions[transaction_index].type , "Transfer") == 0)
    {
         const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
        const gchar *date = gtk_entry_get_text(GTK_ENTRY(entries[1]));
        const gchar *from_account = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[2]));
        gint from_account_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[2]));
        const gchar *to_account = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[3]));
        gint to_account_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[3]));
        const gchar *value_str = gtk_entry_get_text(GTK_ENTRY(entries[4]));

        if (strlen(name) == 0 || strlen(date) == 0 || from_account == NULL || to_account == NULL ||strlen(value_str) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if (!valid_date(date)) {//verify if the date is valid
        show_error("Invalid date!");
    }else if(!valid_amount(value_str)){//verify if the amount is valid
        show_error("Invalid amount!");
    }else{
        accounts[from_account_index + 1].value += transactions[transaction_index].value;
        accounts[to_account_index + 1].value -= transactions[transaction_index].value;

        char initial_name[25];
        g_strlcpy(initial_name , transactions[transaction_index].name , sizeof(initial_name));

        g_strlcpy(transactions[transaction_index].name , name , sizeof(transactions[transaction_index].name));
        g_strlcpy(transactions[transaction_index].date, date, sizeof(transactions[transaction_index].date));
        g_strlcpy(transactions[transaction_index].to_account, to_account, sizeof(transactions[transaction_index].to_account));
        transactions[transaction_index].to_account_index = to_account_index;
        g_strlcpy(transactions[transaction_index].from_account, from_account, sizeof(transactions[transaction_index].from_account));
        transactions[transaction_index].from_account_index = from_account_index;
        g_strlcpy(transactions[transaction_index].to_partner, "-1", sizeof(transactions[transaction_index].to_partner));
        transactions[transaction_index].to_partner_index = -1;
        transactions[transaction_index].value = atof(value_str);
        accounts[from_account_index + 1].value -= transactions[transaction_index].value;
        accounts[to_account_index + 1].value += transactions[transaction_index].value;

        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Edited '%s' transaction | Type: %s | Name: %s | Date: %s | To Account: %s | From Account: %s | Value: %.2f",
                                                        initial_name,
                                                        transactions[transaction_number].type,
                                                        transactions[transaction_number].name,
                                                        transactions[transaction_number].date,
                                                        transactions[transaction_number].to_account,
                                                        transactions[transaction_number].from_account,
                                                        transactions[transaction_number].value);
    }
    }
   
    show_transactions_in_box();

    
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(edit_transaction_button));
     if (show_editable_window_global) {
        gtk_widget_destroy(show_editable_window_global);
    }
}

void edit_transaction(GtkWidget *transaction_button, gpointer data)
{
    int transactions_index = GPOINTER_TO_INT(data);

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    GtkWidget **entries = (GtkWidget **)g_malloc(6 * sizeof(GtkWidget *));//Create a array to store the users input

    entries[0] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_entry_set_text(GTK_ENTRY(entries[0]), transactions[transactions_index].name);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);


    GtkWidget *date_label = gtk_label_new("Date:");
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_entry_set_text(GTK_ENTRY(entries[1]), transactions[transactions_index].date);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);
    if (strcmp(transactions[transactions_index].type , "Income") == 0)
    {
        GtkWidget *from_label = gtk_label_new("From:");
        gtk_grid_attach(GTK_GRID(grid), from_label, 0, 2, 1, 1);

        entries[2] = gtk_combo_box_text_new();
                for (int i = 1 ; i <= partners_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), partners[i].name);
    }
   
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), "Others");
        gtk_combo_box_set_active(GTK_COMBO_BOX(entries[2]), transactions[transactions_index].to_partner_index);
        gtk_grid_attach(GTK_GRID(grid), entries[2], 1, 2, 1, 1);

        GtkWidget *notes_label = gtk_label_new("Account:");
        gtk_grid_attach(GTK_GRID(grid), notes_label, 0, 3, 1, 1);
    }
    if (strcmp(transactions[transactions_index].type , "Expense") == 0)
    {
        GtkWidget *to_label = gtk_label_new("To:");
        gtk_grid_attach(GTK_GRID(grid), to_label, 0, 2, 1, 1);

        entries[2] = gtk_combo_box_text_new();
         for (int i = 1 ; i <= partners_number ; i++)
        {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), partners[i].name);
        }
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), "Others");
        gtk_combo_box_set_active(GTK_COMBO_BOX(entries[2]), transactions[transactions_index].to_partner_index);
        gtk_grid_attach(GTK_GRID(grid), entries[2], 1, 2, 1, 1);

        GtkWidget *notes_label = gtk_label_new("Account:");
        gtk_grid_attach(GTK_GRID(grid), notes_label, 0, 3, 1, 1);
    }
    if (strcmp(transactions[transactions_index].type , "Transfer") == 0)
    {
        GtkWidget *from_account_label = gtk_label_new("From account:");
        gtk_grid_attach(GTK_GRID(grid), from_account_label, 0, 2, 1, 1);

        entries[2] = gtk_combo_box_text_new();
        for (int i = 1 ; i <= accounts_number ; i++)
        {
            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), accounts[i].name);
        }
        gtk_combo_box_set_active(GTK_COMBO_BOX(entries[2]), transactions[transactions_index].from_account_index);
        gtk_grid_attach(GTK_GRID(grid), entries[2], 1, 2, 1, 1);

        GtkWidget *notes_label = gtk_label_new("To Account:");
        gtk_grid_attach(GTK_GRID(grid), notes_label, 0, 3, 1, 1);
    }
    

    entries[3] = gtk_combo_box_text_new();
    for (int i = 1 ; i <= accounts_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[3]), accounts[i].name);
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(entries[3]), transactions[transactions_index].to_account_index);
    gtk_grid_attach(GTK_GRID(grid), entries[3], 1, 3, 1, 1);

    GtkWidget *value_label = gtk_label_new("Value:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 4, 1, 1);

    entries[4] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[4]), 30);
    const char* value_str = g_strdup_printf("%f", transactions[transactions_index].value);
    gtk_entry_set_text(GTK_ENTRY(entries[4]), value_str);
    gtk_grid_attach(GTK_GRID(grid), entries[4], 1, 4, 1, 1);

    int *transactions_index_gtk = (int *)g_malloc(sizeof(int));
    *transactions_index_gtk = transactions_index; 
    entries[5] = (GtkWidget *)transactions_index_gtk;    

    GtkWidget *edit_transaction_button = gtk_button_new_with_label("Edit transaction");
    g_signal_connect(edit_transaction_button, "clicked", G_CALLBACK(edit_transaction_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), edit_transaction_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}


void show_edittable_transactions(GtkWidget *edit_transaction_button, gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management application");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);
        for(int i = 1; i <= transaction_number ; i++)
       {
            if (strcmp(transactions[i].type , "Income") == 0)
            {
                gchar *button_text = g_strdup_printf("Type: %s\nName: %s\nDate: %s\nFrom: %s\nAccount: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].to_partner,
                                                                transactions[i].to_account,
                                                                transactions[i].value);
                GtkWidget *transaction_button = gtk_button_new_with_label(button_text);
                g_signal_connect(transaction_button, "clicked", G_CALLBACK(edit_transaction), GINT_TO_POINTER(i));
                gtk_box_pack_start(GTK_BOX(vbox), transaction_button, TRUE, TRUE, 0);
                g_free(button_text);                                               
            }
            if (strcmp(transactions[i].type , "Expense") == 0)
            {
                gchar *button_text = g_strdup_printf("Type: %s\nName: %s\nDate: %s\nTo: %s\nAccount: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].to_partner,
                                                                transactions[i].to_account,
                                                                transactions[i].value);
                GtkWidget *transaction_button = gtk_button_new_with_label(button_text);
                g_signal_connect(transaction_button, "clicked", G_CALLBACK(edit_transaction), GINT_TO_POINTER(i));
                gtk_box_pack_start(GTK_BOX(vbox), transaction_button, TRUE, TRUE, 0);
                g_free(button_text);
            }
            if (strcmp(transactions[i].type , "Transfer") == 0)
            {
                gchar *button_text = g_strdup_printf("Type: %s\nName: %s\nDate: %s\nFrom: %s\nTo: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].from_account,
                                                                transactions[i].to_account,
                                                                transactions[i].value);
                GtkWidget *transaction_button = gtk_button_new_with_label(button_text);
                g_signal_connect(transaction_button, "clicked", G_CALLBACK(edit_transaction), GINT_TO_POINTER(i));
                gtk_box_pack_start(GTK_BOX(vbox), transaction_button, TRUE, TRUE, 0);
                g_free(button_text);
            }
            
            
       }

    show_editable_window_global = main_window;
    gtk_widget_show_all(main_window);
}

void delete_transaction_dynamic(int index)
{
     
    if (strcmp(transactions[index].type , "Income") == 0)
    {
        accounts[transactions[index].to_account_index + 1].value -= transactions[index].value;
    }
    if (strcmp(transactions[index].type , "Expense") == 0)
    {
        accounts[transactions[index].to_account_index + 1].value += transactions[index].value;
    }
    if (strcmp(transactions[index].type , "Transfer") == 0)
    {
        accounts[transactions[index].to_account_index + 1].value -= transactions[index].value;
        accounts[transactions[index].from_account_index + 1].value += transactions[index].value;
    }

    logs_number++;
    audit_labels[logs_number] = g_strdup_printf("Deleted '%s' transaction! ",
                                                        transactions[index].name);
    
    for (int i = index ; i <= transaction_number ; i++)
        transactions[i] = transactions[i + 1];
    transaction_number--;
    show_transactions_in_box();

    if (show_editable_window_global) {
        gtk_widget_destroy(show_editable_window_global);
    }
}

void delete_transaction(GtkWidget *delete_transaction_button, gpointer data)
{   
    int transaction_index = GPOINTER_TO_INT(data);
    GtkWidget *dialog;
    GtkResponseType response;

 
    dialog = gtk_message_dialog_new(GTK_WINDOW(main_window_global),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "Are you sure you want to delete this account?");

   
    gtk_window_set_title(GTK_WINDOW(dialog), "Delete Account");


    response = gtk_dialog_run(GTK_DIALOG(dialog));

  
    if (response == GTK_RESPONSE_YES) {
        delete_transaction_dynamic(transaction_index);
    }

   
    gtk_widget_destroy(dialog);
}

void show_deletable_transactions(GtkWidget *delete_transaction_button, gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management application");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);
        for(int i = 1; i <= transaction_number ; i++)
       {
            if (strcmp(transactions[i].type , "Income") == 0)
            {
                gchar *button_text = g_strdup_printf("Type: %s\nName: %s\nDate: %s\nFrom: %s\nAccount: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].to_partner,
                                                                transactions[i].to_account,
                                                                transactions[i].value);
                GtkWidget *transaction_button = gtk_button_new_with_label(button_text);
                g_signal_connect(transaction_button, "clicked", G_CALLBACK(delete_transaction), GINT_TO_POINTER(i));
                gtk_box_pack_start(GTK_BOX(vbox), transaction_button, TRUE, TRUE, 0);
                g_free(button_text);                                               
            }
            if (strcmp(transactions[i].type , "Expense") == 0)
            {
                gchar *button_text = g_strdup_printf("Type: %s\nName: %s\nDate: %s\nTo: %s\nAccount: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].to_partner,
                                                                transactions[i].to_account,
                                                                transactions[i].value);
                GtkWidget *transaction_button = gtk_button_new_with_label(button_text);
                g_signal_connect(transaction_button, "clicked", G_CALLBACK(delete_transaction), GINT_TO_POINTER(i));
                gtk_box_pack_start(GTK_BOX(vbox), transaction_button, TRUE, TRUE, 0);
                g_free(button_text);
            }
            if (strcmp(transactions[i].type , "Transfer") == 0)
            {
                gchar *button_text = g_strdup_printf("Type: %s\nName: %s\nDate: %s\nFrom: %s\nTo: %s\nValue: %.2f lei\n",
                                                                transactions[i].type,
                                                                transactions[i].name,
                                                                transactions[i].date,
                                                                transactions[i].from_account,
                                                                transactions[i].to_account,
                                                                transactions[i].value);
                GtkWidget *transaction_button = gtk_button_new_with_label(button_text);
                g_signal_connect(transaction_button, "clicked", G_CALLBACK(delete_transaction), GINT_TO_POINTER(i));
                gtk_box_pack_start(GTK_BOX(vbox), transaction_button, TRUE, TRUE, 0);
                g_free(button_text);
            }
            
            
       }

    show_editable_window_global = main_window;
    gtk_widget_show_all(main_window);
}


void transactions_management(GtkWidget *manage_transactions_button , gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window) , "Fincancial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window) , 100);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1920, 1080);
    //g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

     GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "window { background-image: url('bkg.jpg'); }", -1, NULL);

    // Apply the CSS provider to the window
    GtkStyleContext *context = gtk_widget_get_style_context(main_window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 200); // 200 is the spacing between children
    gtk_container_add(GTK_CONTAINER(main_window), hbox);

    // Create the left box
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), left_box, FALSE, FALSE, 0);

    
     GtkWidget *image = gtk_image_new_from_file("main_bank2.jpg");
    if (image != NULL) {
        gtk_box_pack_start(GTK_BOX(left_box), image, FALSE, FALSE, 0);
    } else {
        g_print("Error loading the image.\n");
    }  

    // Create the first text label
   GtkWidget *label1 = gtk_label_new("Financial Management Application");
    const gchar *css = "label { font-size: 30px; color:yellow; font-weight: bold;}";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label1);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label1, FALSE, FALSE, 0);

    // Create the second text label
    GtkWidget *label2 = gtk_label_new(g_strdup_printf("Welcome %s" , username));
    css = "label { font-size: 24px; font-style: italic; color: yellow;  font-weight: bold;}";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label2);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label2, FALSE, FALSE, 0);

    GtkWidget *middle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), middle_box, FALSE, FALSE, 0);



    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(hbox), scrolledWindow, TRUE, TRUE, 0);

    previous_transactions_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), previous_transactions_box);

    

    GtkWidget *add_transaction_button = gtk_button_new_with_label("Add a new transactions");
    g_signal_connect(add_transaction_button, "clicked", G_CALLBACK(add_transaction), NULL);
    GtkWidget *edit_transaction_button = gtk_button_new_with_label("Edit a transaction");
    g_signal_connect(edit_transaction_button, "clicked", G_CALLBACK(show_edittable_transactions), NULL);
    GtkWidget *delete_transaction_button = gtk_button_new_with_label("Delete a transaction");
    g_signal_connect(delete_transaction_button, "clicked", G_CALLBACK(show_deletable_transactions), NULL);
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    g_signal_connect(back_button, "clicked", G_CALLBACK(back), NULL);

    gtk_box_pack_start(GTK_BOX(middle_box), add_transaction_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), edit_transaction_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), delete_transaction_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), back_button, FALSE, FALSE, 0);

   
    show_transactions_in_box();
    gtk_widget_hide(main_window_global);
    gtk_widget_show_all(main_window);
}

void add_partner_dynamic(GtkWidget *submit_button, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *details = gtk_entry_get_text(GTK_ENTRY(entries[1]));

    if (strlen(name) == 0 || strlen(details) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else{
        partners_number++;
        g_strlcpy(partners[partners_number].name, name, sizeof(partners[partners_number].name));
        g_strlcpy(partners[partners_number].details, details, sizeof(partners[partners_number].details));

        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Added a new partner | Name: %s | Details: %s",
                                                        partners[partners_number].name,
                                                        partners[partners_number].details);
    }
    show_partners_in_box();

    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(submit_button));
}


void add_partner(GtkWidget *add_partner_button, gpointer data) {

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
    GtkWidget **entries = (GtkWidget **)g_malloc(3 * sizeof(GtkWidget *));//Create an array to store the users input

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter the partner's name");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *value_label = gtk_label_new("Details:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter details");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    GtkWidget *submit_button = gtk_button_new_with_label("Submit");
    g_signal_connect(submit_button, "clicked", G_CALLBACK(add_partner_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), submit_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}


void edit_partner_dynamic(GtkWidget *edit_partner_button, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *details = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    int *partners_number_pointer = (int *)entries[2];
    int partner_number = *partners_number_pointer;

    if (strlen(name) == 0 || strlen(details) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else{

        char initial_name[25];
         g_strlcpy(initial_name, partners[partner_number].name, sizeof(initial_name));
        g_strlcpy(partners[partner_number].name, name, sizeof(partners[partner_number].name));
        g_strlcpy(partners[partner_number].details, details, sizeof(partners[partner_number].details));

        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Edited '%s' partner | Name: %s | Details: %s",
                                                        initial_name,
                                                        partners[partners_number].name,
                                                        partners[partners_number].details);
    }
    show_partners_in_box();

    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(edit_partner_button));
     if (show_editable_window_global) {
        gtk_widget_destroy(show_editable_window_global);
    }
}


void edit_partner(GtkWidget *partner_button, gpointer data)
{
    int partners_index = GPOINTER_TO_INT(data);

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);


    GtkWidget **entries = (GtkWidget **)g_malloc(3 * sizeof(GtkWidget *));//Create an array to store the users input

    GtkWidget *name_label = gtk_label_new("Name:");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);

    entries[0] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_entry_set_text(GTK_ENTRY(entries[0]), partners[partners_index].name);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *value_label = gtk_label_new("Details:");
    gtk_grid_attach(GTK_GRID(grid), value_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_entry_set_text(GTK_ENTRY(entries[1]), partners[partners_index].details);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    int *partners_index_gtk = (int *)g_malloc(sizeof(int));
    *partners_index_gtk = partners_index; 
    entries[2] = (GtkWidget *)partners_index_gtk;    

    GtkWidget *edit_partner_button = gtk_button_new_with_label("Edit partner");
    g_signal_connect(edit_partner_button, "clicked", G_CALLBACK(edit_partner_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), edit_partner_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}


void show_edittable_partners(GtkWidget *edit_partner_button, gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management application");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    for (int i = 1; i <= partners_number; ++i) {
        gchar *button_text = g_strdup_printf("Name: %s\nDetails: %s\n",
                                             partners[i].name,
                                             partners[i].details);

        GtkWidget *partner_button = gtk_button_new_with_label(button_text);
        g_signal_connect(partner_button, "clicked", G_CALLBACK(edit_partner), GINT_TO_POINTER(i));
        g_free(button_text);

        gtk_box_pack_start(GTK_BOX(vbox), partner_button, TRUE, TRUE, 0);
    }
    show_editable_window_global = main_window;
    gtk_widget_show_all(main_window);
}

void delete_partner_dynamic(int index)
{   
    logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Deleted  '%s' partner!",
                                                        partners[index].name);
    for (int i = index ; i <= partners_number ; i++)
        partners[i] = partners[i + 1];
    partners_number--;
    show_partners_in_box();

    if (show_editable_window_global) {
        gtk_widget_destroy(show_editable_window_global);
    }
}

void delete_partner(GtkWidget *delete_account_button, gpointer data)
{   
    int partners_index = GPOINTER_TO_INT(data);
    GtkWidget *dialog;
    GtkResponseType response;

    // Create a new dialog
    dialog = gtk_message_dialog_new(GTK_WINDOW(main_window_global),
                                    GTK_DIALOG_MODAL,
                                    GTK_MESSAGE_QUESTION,
                                    GTK_BUTTONS_YES_NO,
                                    "Are you sure you want to delete this partner?");

    // Set the dialog title
    gtk_window_set_title(GTK_WINDOW(dialog), "Delete Partner");

    // Run the dialog and get the user's response
    response = gtk_dialog_run(GTK_DIALOG(dialog));

    // Handle the user's response
    if (response == GTK_RESPONSE_YES) {
        delete_partner_dynamic(partners_index);
    }

    // Destroy the dialog
    gtk_widget_destroy(dialog);
}

void show_deletable_partners(GtkWidget *delete_partner_button, gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management application");
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    for (int i = 1; i <= partners_number; ++i) {
        gchar *button_text = g_strdup_printf("Name %s\nDetails: %s\n",
                                             partners[i].name,
                                             partners[i].details);

        GtkWidget *partner_button = gtk_button_new_with_label(button_text);
        g_signal_connect(partner_button, "clicked", G_CALLBACK(delete_partner), GINT_TO_POINTER(i));
        g_free(button_text);

        gtk_box_pack_start(GTK_BOX(vbox), partner_button, TRUE, TRUE, 0);
    }
    show_editable_window_global = main_window;
    gtk_widget_show_all(main_window);
}

void partners_management(GtkWidget *manage_partners_button , gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window) , "Fincancial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window) , 100);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1920, 1080);
    //g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

     GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "window { background-image: url('bkg.jpg'); }", -1, NULL);

    // Apply the CSS provider to the window
    GtkStyleContext *context = gtk_widget_get_style_context(main_window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 200); // 200 is the spacing between children
    gtk_container_add(GTK_CONTAINER(main_window), hbox);

    // Create the left box
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), left_box, FALSE, FALSE, 0);

    
     GtkWidget *image = gtk_image_new_from_file("main_bank2.jpg");
    if (image != NULL) {
        gtk_box_pack_start(GTK_BOX(left_box), image, FALSE, FALSE, 0);
    } else {
        g_print("Error loading the image.\n");
    }  

    // Create the first text label
    GtkWidget *label1 = gtk_label_new("Financial Management Application");
    const gchar *css = "label { font-size: 30px; color:yellow; font-weight: bold;}";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label1);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label1, FALSE, FALSE, 0);

    // Create the second text label
    GtkWidget *label2 = gtk_label_new(g_strdup_printf("Welcome %s" , username));
    css = "label { font-size: 24px; font-style: italic; color: yellow;font-weight: bold; }";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label2);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label2, FALSE, FALSE, 0);

    GtkWidget *middle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), middle_box, FALSE, FALSE, 0);


    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(hbox), scrolledWindow, TRUE, TRUE, 0);

    previous_partners_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), previous_partners_box);


    

    GtkWidget *add_partner_button = gtk_button_new_with_label("Add a new partner");
    g_signal_connect(add_partner_button, "clicked", G_CALLBACK(add_partner), NULL);
    GtkWidget *edit_partner_button = gtk_button_new_with_label("Edit a partner");
    g_signal_connect(edit_partner_button, "clicked", G_CALLBACK(show_edittable_partners), NULL);
    GtkWidget *delete_partner_button = gtk_button_new_with_label("Delete a partner");
    g_signal_connect(delete_partner_button, "clicked", G_CALLBACK(show_deletable_partners), NULL);
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    g_signal_connect(back_button, "clicked", G_CALLBACK(back), NULL);

    gtk_box_pack_start(GTK_BOX(middle_box), add_partner_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), edit_partner_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), delete_partner_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), back_button, FALSE, FALSE, 0);

   // show_account_in_box();
    show_partners_in_box();
    gtk_widget_hide(main_window_global);
    gtk_widget_show_all(main_window);
}

void import_csv(GtkWidget *widget, gpointer data) //Function to import data from a csv file
{

    //Create and run a dialog that lets you travel through files in your computer and select a file to import data from
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(data),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }

    partners_number =1;
    transaction_number = 1;
    accounts_number = 1;
    
    fscanf(file, "%*[^\n]\n");

    // Read data
    while (fscanf(file, "%99[^,],%99[^,],%f,%99[^\n]\n",
                  accounts[accounts_number].name, accounts[accounts_number].type,
                  &accounts[accounts_number].value, accounts[accounts_number].notes) == 4) {
        accounts_number++;
    }

    fscanf(file, "%*[^\n]\n");

    while (fscanf(file, "%99[^,],%f,%99[^,],%99[^,],%99[^,],%d,%99[^,],%d,%99[^,],%d\n",
                  transactions[transaction_number].date, &transactions[transaction_number].value,
                  transactions[transaction_number].type, transactions[transaction_number].name,
                  transactions[transaction_number].to_partner, &transactions[transaction_number].to_partner_index,
                  transactions[transaction_number].from_account, &transactions[transaction_number].from_account_index,
                  transactions[transaction_number].to_account, &transactions[transaction_number].to_account_index) == 10) {
        transaction_number++;
    }

    fscanf(file, "%*[^\n]\n");

    // Read data
    while (fscanf(file, "%49[^,],%49[^\n]\n", partners[partners_number].name, partners[partners_number].details) == 2) {
        partners_number++;
    }
    accounts_number--;
    partners_number--;
    transaction_number--;
    
     logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Imported data from '%s' file", filename);

    fclose(file);

        g_free(filename);
    }

    

    gtk_widget_destroy(dialog);
}

void export_csv(GtkWidget *widget, gpointer data)//Function to export data to a CSV file
 {
    //Create and run a dialog that lets you travel through files in your computer and select a file to export data to
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Save File",
                                         GTK_WINDOW(data),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Save",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

     FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    // Write header
    fprintf(file, "Name,NumTransactions,Type,Value,Notes\n");

    // Write data
    for (int i = 1; i <= accounts_number; ++i) {
        fprintf(file, "%s,%s,%.2f,%s\n",
                accounts[i].name, accounts[i].type,
                accounts[i].value, accounts[i].notes);
    }

    fprintf(file, "Date,Value,Type,Name,To_Partner,To_Partner_Index,From_Account,From_Account_Index,To_Account,To_Account_Index\n");

    // Write data
    for (int i = 1; i <= transaction_number; ++i) {
        fprintf(file, "%s,%.2f,%s,%s,%s,%d,%s,%d,%s,%d\n",
                transactions[i].date, transactions[i].value, transactions[i].type,
                transactions[i].name, transactions[i].to_partner, transactions[i].to_partner_index,
                transactions[i].from_account, transactions[i].from_account_index,
                transactions[i].to_account, transactions[i].to_account_index);
    }
     fprintf(file, "Name,Details\n");

    // Write data
    for (int i = 1; i <= partners_number; ++i) {
        fprintf(file, "%s,%s\n", partners[i].name, partners[i].details);
    }
    logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Exported data to '%s' file", filename);
    fclose(file);

        g_free(filename);
    }
    

    gtk_widget_destroy(dialog);
}

void audit_logs(GtkWidget *audit_logs_button , gpointer data)//Function to create a new window where to show all logs
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(main_window) , 10);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 600, 400);

    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    // Create a box to hold the labels
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    for(int i = 1 ; i <= logs_number ; i++)
    {
        GtkWidget *label = gtk_label_new(audit_labels[i]);
        gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    }
    gtk_container_add(GTK_CONTAINER(scrolled_window), vbox);

    // Add the scrolled window to the main window
    gtk_container_add(GTK_CONTAINER(main_window), scrolled_window);

    gtk_widget_show_all(main_window);

}

void swapTransactions(Transaction *transactions, int index1, int index2) {
    if (index1 != index2) {
        Transaction temp = transactions[index1];
        transactions[index1] = transactions[index2];
        transactions[index2] = temp;
    }
}

int date_a_before_b(const char *a , const char *b) // function to check if a date is before another date
{
    int day1, month1, year1;
    int day2, month2, year2;
    sscanf(a, "%d/%d/%d", &day1, &month1, &year1);
    sscanf(b, "%d/%d/%d", &day2, &month2, &year2);

    if (year1 < year2 ||
        (year1 == year2 && (month1 < month2 ||
                            (month1 == month2 && day1 < day2)))) {
        return 1; // True
    } else {
        return 0; // False
    }
}

char *month(const char *a) {
    int day, month, year;
    sscanf(a, "%d/%d/%d", &day, &month, &year);

    char *months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    return months[month - 1];
}

int year(const char *a) {
    int day, month, year;
    sscanf(a, "%d/%d/%d", &day, &month, &year);
    return year;
}

void generate_report(GtkWidget *generate_report_button, gpointer data) //Function to show a report about the accounts like how much the user spent and income in every month
{
        //We declare a new array where to store all the transactions but ordered descending by the date

        Transaction *transactions_sorted;
        int transactions_sorted_number = 0;
        transactions_sorted = (Transaction *)malloc(50 * sizeof(Transaction));
   
    for (int i = 1 ; i <= transaction_number ; i++)
    {
            transactions_sorted_number++;
            transactions_sorted[transactions_sorted_number] = transactions[i];
    }

    //Sort the array based on date so we can show the reports that descending on months
  for (int i = 1; i < transactions_sorted_number; i++) {
        for (int j = i + 1; j <= transactions_sorted_number; j++) {
            if (date_a_before_b(transactions_sorted[i].date, transactions_sorted[j].date)) {
                transactions[0] = transactions_sorted[i];
                transactions_sorted[i] = transactions_sorted[j];
                transactions_sorted[j] = transactions[0];
            }
        }
    }
    gtk_container_foreach(GTK_CONTAINER(financial_report_box), (GtkCallback)gtk_widget_destroy, NULL);
    GtkWidget *label1 = gtk_label_new("------------Generated Report------------");

    GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 24px;  font-weight: bold;}", -1, NULL);

        // Apply the CSS provider to the label
        GtkStyleContext *context = gtk_widget_get_style_context(label1);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_box_pack_start(GTK_BOX(financial_report_box), label1, FALSE, FALSE, 0);

    //Create a new struct which will contain the informations for the report
   typedef struct {
    char account[50];
    int year;
    char month[15];
    float incomes;
    float expenses;
} Report;
    Report *report;
    report = (Report *)malloc(50 * sizeof(Report));
    int reports = 0;

    //We fill the struct report with the informations
    for(int i = 1; i <= transactions_sorted_number ; i++)
       {
                 if (strcmp(transactions_sorted[i].type , "Income") == 0)
                {
                    reports++;
           
                    g_strlcpy(report[reports].account, transactions_sorted[i].to_account , sizeof(report[reports].account));
                    g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                    report[reports].year = year(transactions_sorted[i].date);
                    report[reports].incomes += transactions_sorted[i].value;
                }
                else  if (strcmp(transactions_sorted[i].type , "Expense") == 0)
                {
                     reports++;
                   g_strlcpy(report[reports].account, transactions_sorted[i].to_account , sizeof(report[reports].account));
                     g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                    report[reports].year = year(transactions_sorted[i].date);
                    report[reports].expenses += transactions_sorted[i].value;
                }
                else{
                     reports++;
                    g_strlcpy(report[reports].account, transactions_sorted[i].to_account , sizeof(report[reports].account));
                     g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                    report[reports].year = year(transactions_sorted[i].date);
                    report[reports].incomes += transactions_sorted[i].value;

                     reports++;
                    g_strlcpy(report[reports].account, transactions_sorted[i].from_account , sizeof(report[reports].account));
                     g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                    report[reports].year = year(transactions_sorted[i].date);
                    report[reports].expenses += transactions_sorted[i].value;
                }
           }
        
    char months[50];
    strcpy(months , "0");
    int years = 0;
    for (int i = 1 ; i <= reports ; i++)
       {
         if(i > 1)//We check to see if it is the first report. If it is the first report then we also need to show the month
         {
            //We check to see if we are at a new month.If yes then we need to show it, if not then we ignore this
            if (strcmp(report[i].month , report[i - 1].month) != 0 || report[i].year != report[i - 1].year)
            {
                GtkWidget *month_label = gtk_label_new(g_strdup_printf("------------%s %d-------------- \n" , report[i].month , report[i].year));

                GtkCssProvider *provider = gtk_css_provider_new();
                gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

                // Apply the CSS provider to the label
                GtkStyleContext *context = gtk_widget_get_style_context(month_label);
                gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

                gtk_box_pack_start(GTK_BOX(financial_report_box), month_label, FALSE, FALSE, 0);
            }

            //Create a label with the information that need to be shown
            GtkWidget *label = gtk_label_new(g_strdup_printf("Account: %s\nIncomes: %.2f\nExpenses: %.2f\nProfit: %.2f\n", report[i].account , report[i].incomes , report[i].expenses , report[i].incomes - report[i].expenses));
            
             GtkCssProvider *provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

            // Apply the CSS provider to the label
            GtkStyleContext *context = gtk_widget_get_style_context(label);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

            gtk_box_pack_start(GTK_BOX(financial_report_box), label, FALSE, FALSE, 0);

         }
         else
         {
            GtkWidget *month_label = gtk_label_new(g_strdup_printf("------------%s %d-------------- \n" , report[i].month , report[i].year));

            GtkCssProvider *provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

            // Apply the CSS provider to the label
            GtkStyleContext *context = gtk_widget_get_style_context(month_label);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        

            gtk_box_pack_start(GTK_BOX(financial_report_box), month_label, FALSE, FALSE, 0);


            //Create a label with the information that need to be shown
            GtkWidget *label = gtk_label_new(g_strdup_printf("Account: %s\nIncomes: %.2f\nExpenses: %.2f\nProfit: %.2f\n", report[i].account , report[i].incomes , report[i].expenses , report[i].incomes - report[i].expenses));
            
            provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

            // Apply the CSS provider to the label
            context = gtk_widget_get_style_context(label);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
            
            
            gtk_box_pack_start(GTK_BOX(financial_report_box), label, FALSE, FALSE, 0);
         }

       }
       gtk_widget_show_all(financial_report_box);
       
    free(transactions_sorted);
    free(report);

}

void generate_custom_report(GtkWidget *submit_button, gpointer data) //Function to show a custom report about the accounts like how much the user spent and earned in every month
{
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    const gchar *start_date = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *end_date = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *account = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[2]));
  
    gint account_index = gtk_combo_box_get_active(GTK_COMBO_BOX(entries[2]));

    if (strlen(start_date) == 0 || strlen(end_date) == 0 || account == NULL){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else if (!valid_date(start_date) || !valid_date(end_date)) {//verify if the date is valid
        show_error("Invalid date!");
    }else{
        //We declare a new array where to store all the transactions but ordered descending by the date

         Transaction *transactions_sorted;
        int transactions_sorted_number = 0;
        transactions_sorted = (Transaction *)malloc(50 * sizeof(Transaction));
   
    for (int i = 1 ; i <= transaction_number ; i++)
    {
        if(date_a_before_b(transactions[i].date , end_date) && date_a_before_b(start_date , transactions[i].date))
        {
            if(strcmp(transactions[i].to_account , account) == 0 || strcmp(account , "All Accounts") == 0)
            {
                transactions_sorted_number++;
                transactions_sorted[transactions_sorted_number] = transactions[i];
            }
        }
    }
    //Sort the array so we can show the reports in descending order by month
  for (int i = 1; i < transactions_sorted_number; i++) {
        for (int j = i + 1; j <= transactions_sorted_number; j++) {
            if (date_a_before_b(transactions_sorted[i].date, transactions_sorted[j].date)) {
                transactions[0] = transactions_sorted[i];
                transactions_sorted[i] = transactions_sorted[j];
                transactions_sorted[j] = transactions[0];
            }
        }
    }
    gtk_container_foreach(GTK_CONTAINER(financial_report_box), (GtkCallback)gtk_widget_destroy, NULL);
    GtkWidget *label1 = gtk_label_new("------------Generated Report------------");

    GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 24px;  font-weight: bold;}", -1, NULL);

        // Apply the CSS provider to the label
        GtkStyleContext *context = gtk_widget_get_style_context(label1);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_box_pack_start(GTK_BOX(financial_report_box), label1, FALSE, FALSE, 0);
   typedef struct {
     //Create a new struct which will contain the informations for the report
    char account[50];
    int year;
    char month[15];
    float incomes;
    float expenses;
} Report;
    Report *report;
    report = (Report *)malloc(50 * sizeof(Report));
    int reports = 0;
    //Fill the struct with the informations
    for(int i = 1; i <= transactions_sorted_number ; i++)
       {
                 if (strcmp(transactions_sorted[i].type , "Income") == 0)
                {
                    reports++;
           
                    g_strlcpy(report[reports].account, transactions_sorted[i].to_account , sizeof(report[reports].account));
                    g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                    report[reports].year = year(transactions_sorted[i].date);
                    report[reports].incomes += transactions_sorted[i].value;
                }
                else  if (strcmp(transactions_sorted[i].type , "Expense") == 0)
                {
                     reports++;
                   g_strlcpy(report[reports].account, transactions_sorted[i].to_account , sizeof(report[reports].account));
                     g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                    report[reports].year = year(transactions_sorted[i].date);
                    report[reports].expenses += transactions_sorted[i].value;
                }
                else{
                    if(strcmp(transactions[i].to_account , account) == 0 || strcmp(account , "All Accounts") == 0)
                    {
                        reports++;
                        g_strlcpy(report[reports].account, transactions_sorted[i].to_account , sizeof(report[reports].account));
                        g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                        report[reports].year = year(transactions_sorted[i].date);
                        report[reports].incomes += transactions_sorted[i].value;
                    }

                    if(strcmp(transactions[i].to_account , account) == 0 || strcmp(account , "All Accounts") == 0)
                    {
                        reports++;
                        g_strlcpy(report[reports].account, transactions_sorted[i].from_account , sizeof(report[reports].account));
                        g_strlcpy(report[reports].month, month(transactions_sorted[i].date) , sizeof(report[reports].month));
                        report[reports].year = year(transactions_sorted[i].date);
                        report[reports].expenses += transactions_sorted[i].value;
                    }
                }
           }
        
    char months[50];
    strcpy(months , "0");
    int years = 0;
    for (int i = 1 ; i <= reports ; i++)
       {    
         if(i > 1)//We check to see if it is the first report. If it is the first report then we also need to show the month
           { 
            //We check to see if we are at a new month.If yes then we need to show it, if not then we ignore this
            if (strcmp(report[i].month , report[i - 1].month) != 0 || report[i].year != report[i - 1].year)
            {
                GtkWidget *month_label = gtk_label_new(g_strdup_printf("------------%s %d-------------- \n" , report[i].month , report[i].year));

                  GtkCssProvider *provider = gtk_css_provider_new();
                gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

                // Apply the CSS provider to the label
                GtkStyleContext *context = gtk_widget_get_style_context(month_label);
                gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

                gtk_box_pack_start(GTK_BOX(financial_report_box), month_label, FALSE, FALSE, 0);
            }
            GtkWidget *label = gtk_label_new(g_strdup_printf("Account: %s\nIncomes: %.2f\nExpenses: %.2f\nProfit: %.2f\n", report[i].account , report[i].incomes , report[i].expenses , report[i].incomes - report[i].expenses));
            gtk_box_pack_start(GTK_BOX(financial_report_box), label, FALSE, FALSE, 0);

              GtkCssProvider *provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

            // Apply the CSS provider to the label
            GtkStyleContext *context = gtk_widget_get_style_context(label);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


         }
         else
         {
            GtkWidget *month_label = gtk_label_new(g_strdup_printf("------------%s %d-------------- \n" , report[i].month , report[i].year));

              GtkCssProvider *provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

            // Apply the CSS provider to the label
            GtkStyleContext *context = gtk_widget_get_style_context(month_label);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

                gtk_box_pack_start(GTK_BOX(financial_report_box), month_label, FALSE, FALSE, 0);


                GtkWidget *label = gtk_label_new(g_strdup_printf("Account: %s\nIncomes: %.2f\nExpenses: %.2f\nProfit: %.2f\n", report[i].account , report[i].incomes , report[i].expenses , report[i].incomes - report[i].expenses));
            
            provider = gtk_css_provider_new();
            gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 18px;  font-weight: bold; }", -1, NULL);

            // Apply the CSS provider to the label
            context = gtk_widget_get_style_context(label);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
            
            gtk_box_pack_start(GTK_BOX(financial_report_box), label, FALSE, FALSE, 0);
         }

       }
       gtk_widget_show_all(financial_report_box);
       
    free(transactions_sorted);
    free(report);

}
    
    g_free((gchar *)account);
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(submit_button));
    if (transactions_window_global) {
        gtk_widget_destroy(transactions_window_global);
    }
}


void custom_report(GtkWidget *generate_custom_report_button, gpointer data) {

    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 20);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget **entries = (GtkWidget **)g_malloc(6 * sizeof(GtkWidget *));//Create a array to store the users input

    GtkWidget *name_label = gtk_label_new("Start Date(DD/MM/YYYY):");
    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter the start date");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *date_label = gtk_label_new("End Date(DD/MM/YYYY):");
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter the end date");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[1]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    GtkWidget *to_label = gtk_label_new("Account:");
    gtk_grid_attach(GTK_GRID(grid), to_label, 0, 2, 1, 1);

    entries[2] = gtk_combo_box_text_new();
    for (int i = 1 ; i <= accounts_number ; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), accounts[i].name);
    }
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(entries[2]), "All Accounts");
    gtk_grid_attach(GTK_GRID(grid) , entries[2] , 1 , 2 , 1 , 1);


    GtkWidget *generate_report_button = gtk_button_new_with_label("Generate Report");
    g_signal_connect(generate_report_button, "clicked", G_CALLBACK(generate_custom_report), entries);

    gtk_box_pack_start(GTK_BOX(vbox), generate_report_button, FALSE, FALSE, 0);

    gtk_widget_show_all(main_window);
}

void financial_report(GtkWidget *manage_transactions_button , gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window) , "Fincancial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window) , 100);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1920, 1080);
    //g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "window { background-image: url('bkg.jpg'); }", -1, NULL);

    // Apply the CSS provider to the window
    GtkStyleContext *context = gtk_widget_get_style_context(main_window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 200); // 200 is the spacing between children
    gtk_container_add(GTK_CONTAINER(main_window), hbox);

    // Create the left box
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), left_box, FALSE, FALSE, 0);

    
     GtkWidget *image = gtk_image_new_from_file("main_bank2.jpg");
    if (image != NULL) {
        gtk_box_pack_start(GTK_BOX(left_box), image, FALSE, FALSE, 0);
    } else {
        g_print("Error loading the image.\n");
    }  

    // Create the first text label
   GtkWidget *label1 = gtk_label_new("Financial Management Application");
    const gchar *css = "label { font-size: 30px; color:yellow; font-weight: bold;}";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label1);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label1, FALSE, FALSE, 0);

    // Create the second text label
    GtkWidget *label2 = gtk_label_new(g_strdup_printf("Welcome %s" , username));
    css = "label { font-size: 24px; font-style: italic; color: yellow; font-weight: bold;}";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label2);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label2, FALSE, FALSE, 0);

    GtkWidget *middle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), middle_box, FALSE, FALSE, 0);



    GtkWidget *scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
                                   GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(hbox), scrolledWindow, TRUE, TRUE, 0);

    financial_report_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75);
    gtk_container_add(GTK_CONTAINER(scrolledWindow), financial_report_box);
   GtkWidget *label3 = gtk_label_new("------------Generated Report------------");

    provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "label { color: yellow; font-size: 24px;  font-weight: bold;}", -1, NULL);

        // Apply the CSS provider to the label
        context = gtk_widget_get_style_context(label3);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_box_pack_start(GTK_BOX(financial_report_box), label3, FALSE, FALSE, 0);
    

    GtkWidget *generate_report_button = gtk_button_new_with_label("Generate a Financial Report");
    g_signal_connect(generate_report_button, "clicked", G_CALLBACK(generate_report), NULL);
    GtkWidget *generate_custom_report_button = gtk_button_new_with_label("Generate a Custom Financial Report");
    g_signal_connect(generate_custom_report_button, "clicked", G_CALLBACK(custom_report), NULL);
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    g_signal_connect(back_button, "clicked", G_CALLBACK(back), NULL);

    gtk_box_pack_start(GTK_BOX(middle_box), generate_report_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), generate_custom_report_button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(middle_box), back_button, FALSE, FALSE, 0);

    gtk_widget_hide(main_window_global);
    gtk_widget_show_all(main_window);
}

void user_application(GtkWidget *log_in_user_button , gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window) , "Fincancial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window) , 100);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 1920, 1080);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, "window { background-image: url('bkg.jpg'); }", -1, NULL);

    // Apply the CSS provider to the window
    GtkStyleContext *context = gtk_widget_get_style_context(main_window);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 500); // 500 is the spacing between children
    gtk_container_add(GTK_CONTAINER(main_window), hbox);

    // Create the left box
    GtkWidget *left_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), left_box, FALSE, FALSE, 0);

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 75); // 75 is the spacing between children
    gtk_box_pack_start(GTK_BOX(hbox), right_box, FALSE, FALSE, 0);

     GtkWidget *image = gtk_image_new_from_file("main_bank2.jpg");
    if (image != NULL) {
        gtk_box_pack_start(GTK_BOX(left_box), image, FALSE, FALSE, 0);
    } else {
        g_print("Error loading the image.\n");
    }  

    // Create the first text label
    GtkWidget *label1 = gtk_label_new("Financial Management Application");
    const gchar *css = "label { font-size: 30px; color:yellow; font-weight: bold;}";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label1);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label1, FALSE, FALSE, 0);

    // Create the second text label
    GtkWidget *label2 = gtk_label_new(g_strdup_printf("Welcome %s" , username));
    css = "label { font-size: 24px; font-style: italic; color: yellow; font-weight: bold;}";
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1, NULL);
    context = gtk_widget_get_style_context(label2);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
    gtk_box_pack_start(GTK_BOX(left_box), label2, FALSE, FALSE, 0);

    //--------------------------RIGHT BOX----------------------------
    GtkWidget *account_management_button = gtk_button_new_with_label("Account management");
    GtkWidget *manage_transactions_button = gtk_button_new_with_label("Manage transactions");
    GtkWidget *financial_report_button = gtk_button_new_with_label("Financial report");
    GtkWidget *manage_partners_button = gtk_button_new_with_label("Manage bussines partners");
    GtkWidget *import_csv_button = gtk_button_new_with_label("Import data from CSV file");
    GtkWidget *export_csv_button = gtk_button_new_with_label("Export data to CSV file");
    GtkWidget *audit_logs_button = gtk_button_new_with_label("Audit logs");
    
    gtk_box_pack_start(GTK_BOX(right_box), account_management_button, FALSE, FALSE, 0);
    g_signal_connect(account_management_button, "clicked", G_CALLBACK(account_management), NULL);
    gtk_box_pack_start(GTK_BOX(right_box), manage_transactions_button, FALSE, FALSE, 0);
    g_signal_connect(manage_transactions_button, "clicked", G_CALLBACK(transactions_management), NULL);
    gtk_box_pack_start(GTK_BOX(right_box), financial_report_button, FALSE, FALSE, 0);
    g_signal_connect(financial_report_button, "clicked", G_CALLBACK(financial_report), NULL);
    gtk_box_pack_start(GTK_BOX(right_box), manage_partners_button, FALSE, FALSE, 0);
    g_signal_connect(manage_partners_button, "clicked", G_CALLBACK(partners_management), NULL);
    gtk_box_pack_start(GTK_BOX(right_box), import_csv_button, FALSE, FALSE, 0);
    g_signal_connect(import_csv_button, "clicked", G_CALLBACK(import_csv), NULL);
    gtk_box_pack_start(GTK_BOX(right_box), export_csv_button, FALSE, FALSE, 0);
    g_signal_connect(export_csv_button, "clicked", G_CALLBACK(export_csv), NULL);
    gtk_box_pack_start(GTK_BOX(right_box), audit_logs_button, FALSE, FALSE, 0);
    g_signal_connect(audit_logs_button, "clicked", G_CALLBACK(audit_logs), NULL);
    
    

    
    gtk_widget_show_all(main_window);

    gtk_widget_destroy(gtk_widget_get_toplevel(log_in_user_button));
     if (main_window_global) {
        gtk_widget_hide(main_window_global);
    }
    main_window_global = main_window;


}

void verify_password(GtkWidget *log_in_button ,  gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    const gchar *user = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *pass = gtk_entry_get_text(GTK_ENTRY(entries[1]));    
    if (strlen(user) == 0 || strlen(pass) == 0){// verify if all fields are completed
        show_error("Invalid Account!");
    }else{
       
    FILE *file = fopen("creditentials.txt", "r");  // Open the file in read mode

    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
    }

   

    if (fscanf(file, "%s", username) != 1) {
        fprintf(stderr, "Error reading word1 from file\n");
        fclose(file);
    }

    // Read the second word from the file
    if (fscanf(file, "%s", password) != 1) {
        fprintf(stderr, "Error reading word2 from file\n");
        fclose(file);
    }
    if ((strcmp(username , user) == 0) && (strcmp(password , pass) == 0))
    {
        logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Logged in as '%s'", username);
        user_application(log_in_button , NULL);
    }
    else{
        show_error("Invalid Account!");
    }
    fclose(file);
    }
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(log_in_button));
}

void log_in_user(GtkWidget *log_in_user_button, gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    gtk_widget_set_size_request(main_window, 300, 150);
   

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget *username_label = gtk_label_new("Username:");
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);

    GtkWidget **entries = (GtkWidget **)g_malloc(3 * sizeof(GtkWidget *));//Create an array to store the users input

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter the username");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);

    GtkWidget *password_label = gtk_label_new("Password:");
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter the password");
    gtk_entry_set_visibility(GTK_ENTRY(entries[1]), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entries[1]), '*');
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);

    GtkWidget *log_in_button = gtk_button_new_with_label("Log in");
    g_signal_connect(log_in_button, "clicked", G_CALLBACK(verify_password), entries);

    gtk_box_pack_start(GTK_BOX(vbox), log_in_button, FALSE, FALSE, 0);
    
    gtk_widget_show_all(GTK_WIDGET(main_window));
}   

void create_user_dynamic(GtkWidget *create_account_button ,  gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;//Pointer to the array that has the user inputs

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(entries[1]));    
    if (strlen(username) == 0 || strlen(password) == 0){// verify if all fields are completed
        show_error("All fields must be completed!");
    }else{
        FILE *file = fopen("creditentials.txt", "w");

    // Check if the file is successfully opened
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
    }

    // Write username and password to the file
    fprintf(file, "%s\n%s\n", username, password);

    logs_number++;
        audit_labels[logs_number] = g_strdup_printf("Created a new account! | Username: '%s' | Password: %s ", username , password);
    show_error("Account succsesfully created!");
    // Close the file
    fclose(file);
    }
    g_free(entries);
    gtk_widget_destroy(gtk_widget_get_toplevel(create_account_button));
}
void create_user(GtkWidget *create_user_button, gpointer data)
{
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial management app");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    gtk_widget_set_size_request(main_window, 300, 150);
   

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_container_add(GTK_CONTAINER(main_window), vbox);

    GtkWidget *grid = gtk_grid_new();//Create a new grid
    gtk_grid_set_row_spacing(GTK_GRID(grid), 30);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

    GtkWidget **entries = (GtkWidget **)g_malloc(3 * sizeof(GtkWidget *));//Create an array to store the users input

    GtkWidget *username_label = gtk_label_new("New Username:");
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);//Add the label to the column 0 , row 0 of the grid

    entries[0] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[0]), "Enter the username");
    gtk_entry_set_width_chars(GTK_ENTRY(entries[0]), 30);
    gtk_grid_attach(GTK_GRID(grid), entries[0], 1, 0, 1, 1);//Add the entry to the column 1 , row 0 of the grid

    GtkWidget *password_label = gtk_label_new("New Password:");
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);//Add the label to the column 0 , row 1 of the grid

    entries[1] = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entries[1]), "Enter the password");
    gtk_entry_set_visibility(GTK_ENTRY(entries[1]), FALSE);
    gtk_entry_set_invisible_char(GTK_ENTRY(entries[1]), '*');
    gtk_grid_attach(GTK_GRID(grid), entries[1], 1, 1, 1, 1);//Add the entry to the column 1 , row 1 of the grid

    GtkWidget *create_account_button = gtk_button_new_with_label("Create account");
    g_signal_connect(create_account_button, "clicked", G_CALLBACK(create_user_dynamic), entries);

    gtk_box_pack_start(GTK_BOX(vbox), create_account_button, FALSE, FALSE, 0);
    
    gtk_widget_show_all(GTK_WIDGET(main_window));
}   
int main()
{
    gtk_init(NULL , NULL);
    accounts = (Account *)malloc(20 * sizeof(Account));//Create dynamically the struct 
    if (accounts == NULL) {
        // Handle memory allocation failure
        return 1;
    }
    transactions = (Transaction *)malloc(50 * sizeof(Transaction));//Create dynamically the struct 
    if (transactions == NULL) {
        // Handle memory allocation failure
        return 1;
    }
    partners = (Customer *)malloc(20 * sizeof(Customer));//Create dynamically the struct
    if (partners == NULL) {
        // Handle memory allocation failure
        return 1;
    }
     username = (char *)malloc(150 * sizeof(char));//Create dynamically the struct
    if (username == NULL) {
        return 1;
    }

    // Dynamically allocate memory for password
    password = (char *)malloc(150 * sizeof(char));
    if (password == NULL) {
        return 1;
    }

    // Create the main window
    GtkWidget *main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    main_window_global = main_window;
    gtk_window_set_title(GTK_WINDOW(main_window), "Financial Management App");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 600, 500);
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 100);
    g_signal_connect(main_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box to hold the widgets
    GtkWidget *box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box2), 10);
    gtk_container_add(GTK_CONTAINER(main_window), box2);


    // Load an image
    GtkWidget *image = gtk_image_new_from_file("main_bank.jpg");
    if (image != NULL) {
        gtk_box_pack_start(GTK_BOX(box2), image, FALSE, FALSE, 0);
    } else {
        g_print("Error loading the image.\n");
    }
    
    GtkWidget *multilineLabel = gtk_label_new("\n\n\n");
    gtk_box_pack_start(GTK_BOX(box2), multilineLabel, FALSE, FALSE, 0);
    
    //Create buttons so the user can log in or create a new account

    GtkWidget *create_user_button = gtk_button_new_with_label("Create a new account");
    gtk_box_pack_start(GTK_BOX(box2) , create_user_button , FALSE , FALSE , 0);
    g_signal_connect(create_user_button , "clicked" , G_CALLBACK(create_user) , NULL);

    GtkWidget *log_in_user_button = gtk_button_new_with_label("Login in");
    gtk_box_pack_start(GTK_BOX(box2) , log_in_user_button , FALSE , FALSE , 0);
    g_signal_connect(log_in_user_button , "clicked" , G_CALLBACK(log_in_user) , NULL);

    gtk_widget_show_all(main_window);

	gtk_main();

	free(accounts);
    free(transactions);
    free(partners);
    free(username);
    free(password);
                                

    
    
return 0;
}








