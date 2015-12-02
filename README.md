# Tidy View Project

The aim is to have a viewer of a HTML file, with a left and right views. The left is the **before** raw view of the HTML, and the right is the **after** tidied view of the HTML.

In addition each view is to be color coded. At present the color coding is based on C/C++ syntax, but this needs to be changed to a color coding based on HTML syntax.

Also each view will have line numbers in their respective left margins, but at present this is based on the line numbers of each block of data. The idea is to use the new **`libtidy`** feature, `tidySetPrettyPrinterCallback` callback to map the input line numbers to the output line numbers, and the right view will show the line numbers of the input.

Additionally scrolling either view will scroll the other view to the same html source lines.

This is very much a work in progress!

; eof
