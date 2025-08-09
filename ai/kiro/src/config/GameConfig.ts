export interface GameConfig {
  ship: {
    thrustSpeed: number;
    rotationSpeed: number;
    maxSpeed: number;
    friction: number;
    invincibilityTime: number;
  };
  projectile: {
    speed: number;
    maxCount: number;
    lifetime: number;
  };
  asteroid: {
    speeds: {
      large: number;
      medium: number;
      small: number;
    };
    splitCount: { min: number; max: number };
  };
  game: {
    initialLives: number;
    initialAsteroidCount: number;
    waveIncrement: number;
    safeSpawnRadius: number;
  };
  scoring: {
    asteroidPoints: {
      large: number;
      medium: number;
      small: number;
    };
  };
}