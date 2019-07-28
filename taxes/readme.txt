This program will break up the receipts you type in for a specified main category
into sorted sub-categories with separate counts and totals then provide the overall count and total.

The format is as follows:

Year

Main Category name

Number of Subcategories
Number to associate with -> Subcategory name
Number to associate with -> Subcategory name
Number to associate with -> Subcategory name

Receipt Name - Month - Day - Total - Number to associate with Subcategory

Receipt Name - Month - Day - Total - Number to associate with Subcategory

Receipt Name - Month - Day - Total - Number to associate with Subcategory

Receipt Name - Month - Day - Total - Number to associate with Subcategory

. . .

The negative numbers at the end of the list of receipts
-1 -1 -1 -1 -1
should always be the last 'receipt' typed in.

Inside of the Input folder contains a sample list called samplelist.txt with filler values, and inside
the Output folder contains the sorted list called samplelist.completed.txt. You can use these as a reference 
if you like. Keep in mind you can use any name you want for both the input and output, but make sure
they are in the right format.

The sorted list breaks up the receipts read in into different sub-categories with a count
of how many receipts and the total amount for that sub-category in a sorted hierarchy 
that goes by month->day->name->total.

Also, please take notice that if you want to use spaces for the main category, a sub-category or receipt names please use '_' instead of space. For example:

Receipt_Name instead of Receipt Name

To use this program, it requires that you:

1) Gather and break up all of your tax receipts into 'main categories'.
2) Type the required information into the file to be read from (samplelist.txt).