# Asteroids Game

A TypeScript implementation of the classic Asteroids arcade game using Phaser 3.

## Project Setup

This project uses:
- **TypeScript** for type-safe development
- **Phaser 3** for game engine and graphics
- **Webpack** for bundling and development server
- **Jest** for testing (configured but not yet implemented)

## Development

### Prerequisites
- Node.js (v18.12.1 or higher)
- npm

### Installation
```bash
npm install
```

### Available Scripts

- `npm run dev` - Start development server with hot reload
- `npm run build` - Build production bundle
- `npm run test` - Run tests
- `npm run clean` - Clean build directory

### Development Server
The development server runs on `http://localhost:3000` and includes:
- Hot module replacement
- Automatic browser opening
- Source maps for debugging

## Project Structure

```
src/
├── index.ts          # Main entry point
├── index.html        # HTML template
├── styles.css        # Global styles
└── test-setup.ts     # Jest test configuration

dist/                 # Build output (generated)
node_modules/         # Dependencies
```

## Game Features (Planned)

This implementation will include:
- Player spaceship with rotation and thrust controls
- Asteroid field with collision detection
- Shooting mechanics with bullet physics
- Score tracking and lives system
- Progressive difficulty levels
- Sound effects and visual feedback

## Development Status

✅ Project structure and build system setup
⏳ Game implementation in progress

## License

MIT