import { GameConfig } from '../config/GameConfig';
import defaultConfig from '../config/default-config.json';

export class ConfigLoader {
  private static instance: ConfigLoader;
  private config: GameConfig;

  private constructor() {
    this.config = defaultConfig as GameConfig;
  }

  public static getInstance(): ConfigLoader {
    if (!ConfigLoader.instance) {
      ConfigLoader.instance = new ConfigLoader();
    }
    return ConfigLoader.instance;
  }

  public getConfig(): GameConfig {
    return { ...this.config };
  }

  public async loadConfig(configPath?: string): Promise<GameConfig> {
    if (configPath) {
      try {
        const response = await fetch(configPath);
        const customConfig = await response.json();
        this.config = { ...this.config, ...customConfig };
      } catch (error) {
        console.warn('Failed to load custom config, using defaults:', error);
      }
    }
    return this.getConfig();
  }

  public updateConfig(partialConfig: Partial<GameConfig>): void {
    this.config = { ...this.config, ...partialConfig };
  }
}