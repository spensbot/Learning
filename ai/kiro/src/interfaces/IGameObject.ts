export interface IGameObject {
  update(delta: number): void;
  destroy(): void;
  getPosition(): { x: number; y: number };
  getVelocity(): { x: number; y: number };
}