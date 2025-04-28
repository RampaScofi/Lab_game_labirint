#include <gtest/gtest.h>
#include "../Model.h"
#include <fstream>
#include <stdexcept>

class ModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаёт корректный тестовый уровень
        std::ofstream file("test_level.txt");
        file << "6x3\n";
        file << "######\n";
        file << "#A G #\n";
        file << "#   F#\n";
        file.close();
    }

    void TearDown() override {
        // Удаляет временные файлы
        std::remove("test_level.txt");
        std::remove("invalid_level.txt");
        std::remove("empty_level.txt");
        std::remove("short_line_level.txt");
        std::remove("invalid_char_level.txt");
        std::remove("multiple_players_level.txt");
        std::remove("no_player_level.txt");
        std::remove("game.log");
    }
};

TEST_F(ModelTest, LoadLevel_Success) {
    Model model;
    model.loadLevel("test_level.txt");
    EXPECT_EQ(6, model.getLevelWidth());
    EXPECT_EQ(3, model.getLevelHeight());
    EXPECT_EQ(1, model.getPlayerX());
    EXPECT_EQ(1, model.getPlayerY());
    EXPECT_EQ(3, model.getAppleX());
    EXPECT_EQ(1, model.getAppleY());
    EXPECT_EQ(4, model.getFinishX());
    EXPECT_EQ(2, model.getFinishY());
    const auto& map = model.getMap();
    EXPECT_STREQ("######", map[0].c_str());
    EXPECT_STREQ("#A G #", map[1].c_str());
    EXPECT_STREQ("#   F#", map[2].c_str());
}

TEST_F(ModelTest, LoadLevel_FileNotFound_Throws) {
    Model model;
    EXPECT_THROW(model.loadLevel("nonexistent.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Не удалось открыть файл") != std::string::npos);
}

TEST_F(ModelTest, LoadLevel_EmptyFile_Throws) {
    std::ofstream file("empty_level.txt");
    file.close();
    Model model;
    EXPECT_THROW(model.loadLevel("empty_level.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Файл пуст") != std::string::npos);
}

TEST_F(ModelTest, LoadLevel_NonNumericDimensions_Throws) {
    std::ofstream file("invalid_level.txt");
    file << "ax3\n";
    file.close();
    Model model;
    EXPECT_THROW(model.loadLevel("invalid_level.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Неверный формат первой строки") != std::string::npos);
}

TEST_F(ModelTest, LoadLevel_NegativeDimensions_Throws) {
    std::ofstream file("invalid_level.txt");
    file << "-5x3\n";
    file.close();
    Model model;
    EXPECT_THROW(model.loadLevel("invalid_level.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Недопустимые размеры") != std::string::npos);
}

TEST_F(ModelTest, LoadLevel_ShortLine_Throws) {
    std::ofstream file("short_line_level.txt");
    file << "5x3\n";
    file << "#####\n";
    file << "#A G#\n";
    file << "#F#\n";
    file.close();
    Model model;
    EXPECT_THROW(model.loadLevel("short_line_level.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Недостаточно символов в строке 3") != std::string::npos);
}

TEST_F(ModelTest, LoadLevel_InvalidChar_Throws) {
    std::ofstream file("invalid_char_level.txt");
    file << "5x3\n";
    file << "#####\n";
    file << "#AXG#\n";
    file << "#  F#\n";
    file.close();
    Model model;
    EXPECT_THROW(model.loadLevel("invalid_char_level.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Недопустимый символ") != std::string::npos);
}

TEST_F(ModelTest, LoadLevel_MultiplePlayers_Throws) {
    std::ofstream file("multiple_players_level.txt");
    file << "5x3\n";
    file << "#####\n";
    file << "#AAG#\n";
    file << "#  F#\n";
    file.close();
    Model model;
    EXPECT_THROW(model.loadLevel("multiple_players_level.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Обнаружено несколько игроков") != std::string::npos);
}

TEST_F(ModelTest, LoadLevel_NoPlayer_Throws) {
    std::ofstream file("no_player_level.txt");
    file << "5x3\n";
    file << "#####\n";
    file << "#  G#\n";
    file << "#  F#\n";
    file.close();
    Model model;
    EXPECT_THROW(model.loadLevel("no_player_level.txt"), std::runtime_error);
    std::ifstream log("game.log");
    std::string line;
    std::getline(log, line);
    EXPECT_TRUE(line.find("Игрок не найден") != std::string::npos);
}

TEST_F(ModelTest, MovePlayer_EmptyCell_Success) {
    Model model;
    model.loadLevel("test_level.txt");
    bool moved = model.movePlayer(0, 1);
    EXPECT_TRUE(moved);
    EXPECT_EQ(1, model.getPlayerX());
    EXPECT_EQ(2, model.getPlayerY());
}

TEST_F(ModelTest, MovePlayer_Wall_Fails) {
    Model model;
    model.loadLevel("test_level.txt");
    bool moved = model.movePlayer(-1, 0);
    EXPECT_FALSE(moved);
    EXPECT_EQ(1, model.getPlayerX());
    EXPECT_EQ(1, model.getPlayerY());
}

TEST_F(ModelTest, MovePlayer_PushApple_Success) {
    Model model;
    model.loadLevel("test_level.txt");
    bool moved1 = model.movePlayer(1, 0);
    EXPECT_TRUE(moved1);
    EXPECT_EQ(2, model.getPlayerX());
    EXPECT_EQ(1, model.getPlayerY());
    EXPECT_EQ(3, model.getAppleX()); 
    EXPECT_EQ(1, model.getAppleY());

    bool moved2 = model.movePlayer(1, 0);
    EXPECT_TRUE(moved2);
    EXPECT_EQ(3, model.getPlayerX());
    EXPECT_EQ(1, model.getPlayerY());
    EXPECT_EQ(4, model.getAppleX());
    EXPECT_EQ(1, model.getAppleY());
}

TEST_F(ModelTest, MovePlayer_PushAppleIntoWall_Fails) {
    Model model;
    model.loadLevel("test_level.txt");
    bool moved1 = model.movePlayer(1, 0);
    EXPECT_TRUE(moved1);
    EXPECT_EQ(2, model.getPlayerX());
    EXPECT_EQ(1, model.getPlayerY());
    EXPECT_EQ(3, model.getAppleX());
    EXPECT_EQ(1, model.getAppleY());

    bool moved2 = model.movePlayer(1, 0);
    EXPECT_TRUE(moved2);
    EXPECT_EQ(3, model.getPlayerX());
    EXPECT_EQ(1, model.getPlayerY());
    EXPECT_EQ(4, model.getAppleX());
    EXPECT_EQ(1, model.getAppleY());

    bool moved3 = model.movePlayer(1, 0);
    EXPECT_FALSE(moved3);
    EXPECT_EQ(3, model.getPlayerX());
    EXPECT_EQ(1, model.getPlayerY());
    EXPECT_EQ(4, model.getAppleX());
    EXPECT_EQ(1, model.getAppleY());
}

TEST_F(ModelTest, IsFinished_WhenOnFinish_ReturnsTrue) {
    Model model;
    model.loadLevel("test_level.txt");
    model.movePlayer(3, 0);
    model.movePlayer(0, 1);
    EXPECT_TRUE(model.isFinished());
}