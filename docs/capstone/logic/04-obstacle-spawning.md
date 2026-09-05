---
generated_at_commit: "9bd597ca8fefe478a0365a4fbf5273473d470b5a"
generated_at: "2026-09-03"
capstone_version: "5.2.0"
paths_covered:
  - ":(top)nitrosurfer/source/game.c"
---

# Logic: Obstacle Wave Spawner & Fairness Director

## Trigger
`g_game.spawn_timer` ticks down to 0 during active gameplay (`nitrosurfer/source/game.c:451`).

## Rules
1. Instead of independent random rolls, obstacles are generated using a 4-pattern choreographed wave director:
   - **Pattern 0 (Hurdle Focus)**: Roadblock in lane $L$, coin jump arc overhead.
   - **Pattern 1 (Slide Focus)**: Overhead highway sign in lane $L$, coin trail underneath.
   - **Pattern 2 (Split Dodge)**: Two obstacles on flanks, middle lane guaranteed open.
   - **Pattern 3 (Semi-Truck Convoy)**: Heavy truck on one flank, coin line on opposite flank.
2. **Dynamic Oncoming Traffic**: Semi-trucks (`OBS_TRUCK`) advance at $1.4\times$ relative velocity with high-intensity oncoming halogen headlights, distinguishing active traffic from stationary road hazards.
3. **World-Space Symmetric Collision Evaluation**: Collision distance $dx$ is evaluated in pure 8.8 fixed-point world coordinates (`dx = abs(player.x - LANE_X_PLAYER[lane])`), eliminating perspective road-tapering drift and ensuring 100% lateral symmetry.
4. **Impact Physics & Debris**:
   - Nitro smashing destroys hazards, triggering 8 high-velocity splinter particles (`g_debris`).
   - Close-call near misses ($dx \in [hit\_width, 24]$) trigger Doppler audio whoosh (`sfx_close_call()`) and grinding sparks.
   - Fatal impacts initiate a 20-frame arcade wipeout spin-out sequence with hops, sparks, and screen shake before transitioning to Game Over.
5. **Fairness Guarantee**: The spawner validates that across all approaching obstacles at $Z > 55$, at least one lane is guaranteed safe and passable.
6. Interval: Waves spawn every $85..125$ frames, ensuring reaction time is never compressed below human reflex limits.
7. **Player Forward Ground Plane ($Y = 120, Z = 16$)**: The player sports car sits 12px further forward on the tarmac with lane positions at $X \in \{83, 120, 157\}$. This expands the forward field-of-view for spotting oncoming hazards and establishes a 40px visual runway behind the car.
8. **Continuous Bottom-Edge Exit Projection**: Obstacles, coins, and powerups do not clip or freeze at the player plane. Perspective projection smoothly continues past the camera ($Z$ down to $-10$, $Y$ expanding past $160$ to $185$) so hazards roll naturally off the bottom edge of the display before deactivating at $Y \ge 180$ ($Z \le -8$).
