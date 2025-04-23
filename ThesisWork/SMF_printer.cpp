#include "SFML_printer.h"



void printSFMLMatrix(const MatrixT<Cell>& matrix, const Scheme& scheme)
{
	// Set the pixel size for one grid cell
	constexpr size_t cellSize = 30;

	// Calculate the window dimensions based on field parameters
	unsigned int windowWidth = scheme.getFieldCols() * cellSize;
	unsigned int windowHeight = scheme.getFieldRows() * cellSize;

	// Create an SFML window (in SFML 3.0, the title is the first argument)
	sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Placed Cells");



	// Clear the window with a white background
	window.clear(sf::Color::White);

	// Iterate over the matrix
	// matrix[i][j] corresponds to x = j and y = i
	for (size_t i = 0; i < matrix.size(); ++i) {
		for (size_t j = 0; j < matrix[i].size(); ++j) {
			const Cell& cell = matrix[i][j];

			// Create a rectangle shape with size scaled by cellSize
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(cell.width * cellSize, cell.height * cellSize));

			// Set position based on matrix indices: x = j * cellSize, y = i * cellSize
			rect.setPosition(j * cellSize, i * cellSize);

			// Determine a fill color based on cell ID; light gray for invalid cells
			sf::Color fillColor;
			if (cell.is_filler() || cell.m_id == Cell::invalidID) {
				fillColor = sf::Color::White;
			}
			else {
				int colorSeed = cell.m_id % 256;
				fillColor = sf::Color((colorSeed * 53) % 256,
					(colorSeed * 97) % 256,
					(colorSeed * 37) % 256);


				rect.setFillColor(fillColor);
				rect.setOutlineColor(sf::Color::Black);
				//rect.setOutlineThickness(1.0f);
				window.draw(rect);
			}



			// Draw the rectangle on the window

		}
	}

	// Display the rendered frame
	window.display();

	// Main rendering loop
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}
}

