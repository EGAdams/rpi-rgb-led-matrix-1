#
# 081524
#
import os

def main():
    # Open and read the current brightness value from the file
    HOME = os.path.expanduser( "~" )
    bright_file = f"{ HOME }/rpi-rgb-led-matrix/pickle_cpp/brightness.txt"
    # exit if bright_file does not exist
    if not os.path.exists( bright_file ):
        print( f"ERROR: { bright_file } does not exist." )
        return

    try:
        with open( bright_file, "r" ) as file:
            current_brightness = file.read().strip()
    except FileNotFoundError:
        print("brightness.txt not found. Creating a new one.")
        current_brightness = "15"  # default value if file does not exist

    # Get the new brightness value from the user.  If "<enter>" then new brightness = current brightness
    new_brightness = input(f"Brightness is set at {current_brightness}. Enter new value: ").strip()
    if not new_brightness:
        new_brightness = current_brightness
    
    # Validate the new brightness value
    try:
        new_brightness = int(new_brightness)
        if not 1 <= new_brightness <= 100:
            print("Brightness value must be between 1 and 100.")
            return
    except ValueError:
        print("Invalid brightness value. Please enter an integer.")
        return
    new_brightness = str(new_brightness)

    # Write the new brightness value to the file
    with open( bright_file, "w" ) as file:
        file.write(new_brightness)

    print(f"Brightness level updated to {new_brightness}.")

if __name__ == "__main__":
    main()
