#!/bin/bash

# Database file
DB_FILE="student_db.txt"

# Create database file if it doesn't exist
if [ ! -f "$DB_FILE" ]; then
    touch "$DB_FILE"
fi

# Function to clear screen and show header
show_header() {
    clear
    echo "================================"
    echo "    Student Database Manager    "
    echo "================================"
    echo
}

# Function to insert student record
insert_student() {
    show_header
    echo "INSERT NEW STUDENT RECORD"
    echo "------------------------"
    
    # Get student details
    echo -n "Enter Roll No: "
    read roll_no
    
    # Check if roll no already exists
    if grep -q "^$roll_no|" "$DB_FILE"; then
        echo "Error: Roll No already exists!"
        return
    fi
    
    echo -n "Enter Name: "
    read name
    echo -n "Enter Class: "
    read class
    echo -n "Enter Marks: "
    read marks
    
    # Add record to database
    echo "$roll_no|$name|$class|$marks" >> "$DB_FILE"
    echo "Record inserted successfully!"
    
    read -p "Press Enter to continue..."
}

# Function to delete student record
delete_student() {
    show_header
    echo "DELETE STUDENT RECORD"
    echo "--------------------"
    
    echo -n "Enter Roll No to delete: "
    read roll_no
    
    # Check if record exists
    if grep -q "^$roll_no|" "$DB_FILE"; then
        # Create temporary file excluding the record to delete
        grep -v "^$roll_no|" "$DB_FILE" > temp.txt
        mv temp.txt "$DB_FILE"
        echo "Record deleted successfully!"
    else
        echo "Error: Roll No not found!"
    fi
    
    read -p "Press Enter to continue..."
}

# Function to update student record
update_student() {
    show_header
    echo "UPDATE STUDENT RECORD"
    echo "--------------------"
    
    echo -n "Enter Roll No to update: "
    read roll_no
    
    # Check if record exists
    if grep -q "^$roll_no|" "$DB_FILE"; then
        echo "Current record:"
        grep "^$roll_no|" "$DB_FILE" | tr '|' '\t'
        
        echo -n "Enter new Name (or press Enter to keep current): "
        read name
        echo -n "Enter new Class (or press Enter to keep current): "
        read class
        echo -n "Enter new Marks (or press Enter to keep current): "
        read marks
        
        # Get current values
        OLD_IFS=$IFS
        IFS='|'
        read -r old_roll old_name old_class old_marks <<< $(grep "^$roll_no|" "$DB_FILE")
        IFS=$OLD_IFS
        
        # Use new values or keep old ones if empty
        name=${name:-$old_name}
        class=${class:-$old_class}
        marks=${marks:-$old_marks}
        
        # Update record
        sed -i "/^$roll_no|/c\\$roll_no|$name|$class|$marks" "$DB_FILE"
        echo "Record updated successfully!"
    else
        echo "Error: Roll No not found!"
    fi
    
    read -p "Press Enter to continue..."
}

# Function to search student record
search_student() {
    show_header
    echo "SEARCH STUDENT RECORD"
    echo "--------------------"
    
    echo "Search by:"
    echo "1. Roll No"
    echo "2. Name"
    echo -n "Enter choice (1-2): "
    read choice
    
    case $choice in
        1)
            echo -n "Enter Roll No: "
            read search_term
            search_pattern="^$search_term|"
            ;;
        2)
            echo -n "Enter Name: "
            read search_term
            search_pattern="|$search_term|"
            ;;
        *)
            echo "Invalid choice!"
            read -p "Press Enter to continue..."
            return
            ;;
    esac
    
    echo -e "\nSearch Results:"
    echo "----------------------------------------"
    echo -e "Roll No\tName\tClass\tMarks"
    echo "----------------------------------------"
    
    # Search and display results
    grep -i "$search_pattern" "$DB_FILE" | tr '|' '\t'
    
    if [ $? -ne 0 ]; then
        echo "No records found!"
    fi
    
    read -p "Press Enter to continue..."
}

# Function to display all records
display_all() {
    show_header
    echo "ALL STUDENT RECORDS"
    echo "------------------"
    echo -e "Roll No\tName\tClass\tMarks"
    echo "----------------------------------------"
    
    if [ -s "$DB_FILE" ]; then
        cat "$DB_FILE" | tr '|' '\t'
    else
        echo "No records found!"
    fi
    
    read -p "Press Enter to continue..."
}

# Main menu loop
while true; do
    show_header
    echo "1. Insert Student Record"
    echo "2. Delete Student Record"
    echo "3. Update Student Record"
    echo "4. Search Student Record"
    echo "5. Display All Records"
    echo "6. Exit"
    echo
    echo -n "Enter choice (1-6): "
    read choice
    
    case $choice in
        1) insert_student ;;
        2) delete_student ;;
        3) update_student ;;
        4) search_student ;;
        5) display_all ;;
        6) 
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice! Please try again."
            read -p "Press Enter to continue..."
            ;;
    esac
done
