rm ../images/chords/*.svg

# generate SVG files
gcc -Wall -Wpedantic -Werror website.c chord.c -o generate-chords && ./generate-chords

# insert SVG files into HTML template
python3 generate_chords_html.py
