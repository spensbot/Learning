import { GameState } from '../interfaces/GameState';

export class GameStateManager {
  private state: GameState;

  constructor() {
    this.state = this.getInitialState();
  }

  private getInitialState(): GameState {
    return {
      score: 0,
      lives: 3,
      wave: 1,
      isGameOver: false,
      isPaused: false,
      asteroidCount: 0
    };
  }

  public getState(): GameState {
    return { ...this.state };
  }

  public updateScore(points: number): void {
    this.state.score += points;
  }

  public loseLife(): void {
    this.state.lives = Math.max(0, this.state.lives - 1);
    if (this.state.lives === 0) {
      this.state.isGameOver = true;
    }
  }

  public nextWave(): void {
    this.state.wave += 1;
  }

  public setWave(wave: number): void {
    this.state.wave = wave;
  }

  public setAsteroidCount(count: number): void {
    this.state.asteroidCount = count;
  }

  public setPaused(paused: boolean): void {
    this.state.isPaused = paused;
  }

  public reset(): void {
    this.state = this.getInitialState();
  }

  public isGameOver(): boolean {
    return this.state.isGameOver;
  }

  public isPaused(): boolean {
    return this.state.isPaused;
  }
}