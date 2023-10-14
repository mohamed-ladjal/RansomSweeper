@echo off
g++ ransomware/ransom.cpp -o ransomware/not_virus -LC:\Users\ASUS\SFML\lib -IC:\Users\ASUS\SFML\include  -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lssl -lcrypto