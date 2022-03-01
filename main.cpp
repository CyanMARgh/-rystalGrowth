#include <iostream>
#include <stdint.h>
#include <SFML/Graphics.hpp>

class clusterSpace {
public:
	sf::Sprite spr;
	sf::Texture tex;
	sf::Image img;
	uint32_t *map;
	uint16_t S, mh, D;

	clusterSpace(uint16_t S_, uint16_t D_ = 10) {
		D = D_;
		S = S_;
		mh = S > D ? 0 : S - D;
		img.create(S, S);
		map = (uint32_t *) (img.getPixelsPtr());
		tex.create(S, S);
		spr = sf::Sprite(tex);
		for (uint32_t x = 0; x < S; x++) {
			place(x, S - 1);
		}
	}
	void clear() {
		for (uint16_t y = 0; y < S; y++) {
			for (uint16_t x = 0; x < S; x++) {
				map[x + S * y] = y == S - 1 ? 0xffffffff : 0xff000000;
			}
		}
	}
	void draw(sf::RenderWindow *w) {
		tex.loadFromImage(img);
		w->draw(spr);
	}
	bool get(uint16_t x, uint16_t y) {
		return map[x + S * y] & 0x00ffffff;
	}
	void place(uint16_t x, uint16_t y) {
		map[x + S * y] = 0xffffffff;
	}
	bool spawn() {
		uint16_t x = rand() % S;
		uint16_t y = 0;
		if (get(x, y))return false;
		while (1) {
			bool t[5];
			t[0] = x == 0 || !get(x - 1, y);
			t[1] = x == S - 1 || !get(x + 1, y);
			t[2] = y <= mh || !get(x, y - 1);
			t[3] = y == S - 1 || !get(x, y + 1);
			if (!(t[0] && t[1] && t[2] && t[3])) break;
			t[0] = x == 0;
			t[1] = x == S - 1;
			t[2] = y <= mh;
			t[3] = y == S - 1;
			t[4] = true;
			uint8_t i = 4;
			while (t[i]) i = rand() % 4;
			x += (i == 0 ? -1 : i == 1 ? 1 : 0);
			y += (i == 2 ? -1 : i == 3 ? 1 : 0);
		}
		place(x, y);
		uint16_t mh_ = y - 1 > D ? y - 1 - D : 0;
		mh = mh < mh_ ? mh_ : mh;
		return true;
	}
};

int main() {
	const uint16_t S = 900;
	clusterSpace cs(S);

	sf::RenderWindow window(sf::VideoMode(S, S), "cluster");

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					cs.clear();
				}
			}
		}
		window.clear();
		for (uint32_t i = 0; i < 300; i++) {
			cs.spawn();
		}
		cs.draw(&window);
		window.display();
	}

	return 0;
}
