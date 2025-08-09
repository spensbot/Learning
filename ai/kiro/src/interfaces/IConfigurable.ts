import { GameConfig } from '../config/GameConfig';

export interface IConfigurable {
  applyConfig(config: GameConfig): void;
}