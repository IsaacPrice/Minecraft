#include "chunk.hpp"

#include <typeinfo>

using namespace std;

Chunk blankChunk;

class World {
public:
    World(unsigned long seed, unsigned short renderDistance) {
        this->seed = seed;
        this->renderDistance = renderDistance;

        // Generate the chunks
        GenerateChunks();
    }

    // Renders the world
    void Render(vec3 playerPos) {
        // Update the current chunks
        //UpdateCurrentChunks(playerPos);

        // Render the chunks
        for (int i = 0; i < renderDistance; i++) {
            for (int j = 0; j < renderDistance; j++) {
                currentChunks[i][j].Draw();
            }
        }
    }

    void GenerateChunks() {
        // Resize the outer vector and initialize all chunks to the default value
        currentChunks = std::vector<std::vector<Chunk>>(renderDistance, std::vector<Chunk>(renderDistance, Chunk()));

        /* ORDER OF GENERATION 
            1.
                currentChunks[i][j].worldPos = vec2(i, j);
                currentChunks[i][j].Generate(i, j);
            2.
                currentChunks[i][j].MakeVertexObject();
                currentChunks[i][j].CreateObject();
                currentChunks[i][j].Cleanup();
        */

        // Part one of generation
        for (int i = 0; i < renderDistance; i++) {
            for (int j = 0; j < renderDistance; j++) {
                currentChunks[i][j].chunkPos = { i, j };
                currentChunks[i][j].Generate();
            }
        }

        // Part two of generation
        for (int i = 0; i < renderDistance; i++) {
            for (int j = 0; j < renderDistance; j++) {
                currentChunks[i][j].MakeVertexObject(
                    (i == 0) ? blankChunk : currentChunks[i-1][j],
                    (i == renderDistance - 1) ? blankChunk : currentChunks[i+1][j],
                    (j == 0) ? blankChunk : currentChunks[i][j-1],
                    (j == renderDistance - 1) ? blankChunk : currentChunks[i][j+1]
                );
                currentChunks[i][j].CreateObject();
                currentChunks[i][j].Cleanup();
            }
        }
    }

private:
    // World Settings
    unsigned long seed;

    // User Settings
    unsigned short renderDistance;

    // The chunks
    vector<vector<Chunk>> currentChunks;
};
