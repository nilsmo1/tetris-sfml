#include <array>

struct Point {
    int xPos;
    int yPos;
};

typedef std::array<Point, 4> Tetramino;

const std::array<Tetramino, 4> I_TETRAMINO_ROTATIONS {{
    {{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } }},
    {{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } }},
    {{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 1, 3 } }},
    {{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 3, 1 } }},
}};

const std::array<Tetramino, 4> J_TETRAMINO_ROTATIONS {{
    {{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 0, 2 } }},
    {{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } }},
    {{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 1, 2 } }},
    {{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 2 } }},
}};

const std::array<Tetramino, 4> L_TETRAMINO_ROTATIONS {{
    {{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 2, 0 } }},
    {{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } }},
    {{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 0, 2 } }},
    {{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } }},
}};

const std::array<Tetramino, 4> Z_TETRAMINO_ROTATIONS {{
    {{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } }},
    {{ { 2, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } }},
    {{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } }},
    {{ { 2, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } }},
}};

const std::array<Tetramino, 4> S_TETRAMINO_ROTATIONS {{
    {{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } }},
    {{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } }},
    {{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } }},
    {{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } }},
}};

const std::array<Tetramino, 4> O_TETRAMINO_ROTATIONS {{
    {{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }},
    {{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }},
    {{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }},
    {{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }},
}};

const std::array<Tetramino, 4> T_TETRAMINO_ROTATIONS {{
    {{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } }},
    {{ { 1, 0 }, { 1, 1 }, { 2, 1 }, { 1, 2 } }},
    {{ { 0, 1 }, { 1, 1 }, { 2, 1 }, { 1, 2 } }},
    {{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } }},
}};

const std::array<std::array<Tetramino, 4>, 7> ALL_TETRAMINOS {{
    I_TETRAMINO_ROTATIONS,
    J_TETRAMINO_ROTATIONS,
    L_TETRAMINO_ROTATIONS,
    Z_TETRAMINO_ROTATIONS,
    S_TETRAMINO_ROTATIONS,
    O_TETRAMINO_ROTATIONS,
    T_TETRAMINO_ROTATIONS,
}};
