<script>
	import { onMount } from 'svelte';
	import Player from './Player.svelte';

	let size = 3;
	let errorMsg = '';
	let solveMsg = '';
	let solved = false;
	let thumbX, thumbY
	let cells = [
		{ value: 'N', x: 0, y: 0 },
		{ value: '-', x: 0, y: 1 },
		{ value: 'P', x: 1, y: 0 },
		{ value: 'U', x: 1, y: 1 },
		{ value: 'Z', x: 1, y: 2 },
		{ value: 'Z', x: 2, y: 0 },
		{ value: 'L', x: 2, y: 1 },
		{ value: 'E', x: 2, y: 2 },		
	]
	let initialState;
	let solutionState;
	let puzzleStr = '8 1 6 7 0 4 3 5 2';
	let solutionStr = 'D L U U R R D D L U U R D D L U U R D L'
	let puzzleFontSize;
	$: puzzleFontSize = `${Math.floor(20 / size)}rem`

	let loaded = false;

	let solution;
	let solutionIndex = -1;
	
	const directions = {
		'U': [-1, 0],
		'D': [1, 0],
		'L': [0, -1],
		'R': [0, 1]
	}

	let movingInterval;
	let playing = false;

	let prevPos = [];

	let emojis = ['🧠', '🐊', '🎃', '👁️', '​💀​', '🫀', '​🐐', '​🔥', '⭐', '​👾', '🧟‍♂️', '​​🪲', '​🐲', '🫐', '​🍥', '​🌚', '​💣', '🌸', '​💩'​]

	function isSqrd(n) {
		if (n < 4) return false; // Exclude numbers smaller than 4

		const sqrt = Math.sqrt(n);
		return (sqrt === Math.floor(sqrt)) ? sqrt : -1;
	}

	function getMissingIndex(cells) {
		const helper = new Set(Array.from({ length: size * size }, (_, i) => i));
		cells.forEach(cell => helper.delete(cell.x * size + cell.y));
		const left = helper.values().next().value;
		return [Math.floor(left / size), left % size];
	}

	function getNeighbors(x, y) {
		let res = [];
		if (x > 0) res.push(cells.find(cell => cell.x === x - 1 && cell.y === y));
		if (x < size - 1) res.push(cells.find(cell => cell.x === x + 1 && cell.y === y));
		if (y > 0) res.push(cells.find(cell => cell.x === x && cell.y === y - 1));
		if (y < size - 1) res.push(cells.find(cell => cell.x === x && cell.y === y + 1));
		return res.filter(cell => cell.x != prevPos[0] && cell.y != prevPos[1]);
	}

	function getNextPos(x, y, dx, dy) {
		if (x + dx < 0 || x + dx >= size || y + dy < 0 || y + dy >= size)
			return [-1, -1];
		return [x + dx, y + dy];
	}

	function createSnail(size) {
		let res = Array.from({ length: size }, () => Array.from({ length: size }, () => 0));
		let val = 1; // The value to be inserted
		let rowStart = 0, rowEnd = size - 1;
		let colStart = 0, colEnd = size - 1;

		while (rowStart <= rowEnd && colStart <= colEnd) {
			for (let j = colStart; j <= colEnd; ++j)
				res[rowStart][j] = val++;
			++rowStart;

			for (let i = rowStart; i <= rowEnd; ++i)
				res[i][colEnd] = val++;
			--colEnd;

			for (let j = colEnd; j >= colStart; --j)
				res[rowEnd][j] = val++;
			--rowEnd;

			for (let i = rowEnd; i >= rowStart; --i)
				res[i][colStart] = val++;
			++colStart;
		}
		return res;
	}

	function compareResult() {
		for (let cell of cells) {
			if (parseInt(cell.value) !== solutionState[cell.x][cell.y]) {
				errorMsg = 'Solution is not valid (Wrong Result)'
				return
			}
		}
		solveMsg = 'Puzzle solved!'
	}

	onMount(() => {
        document.documentElement.style.setProperty('--main', '#ff3e00');
		movingInterval = setInterval(() => {
			const [emptyX, emptyY] = getMissingIndex(cells);
			const neighbors = getNeighbors(emptyX, emptyY);
			let picked = neighbors[Math.floor(Math.random() * neighbors.length)];
			picked.x = emptyX;
			picked.y = emptyY;
			prevPos = [emptyX, emptyY];
			cells = cells
		}, 800)
	})
</script>

<main>
	<div class="cell-container">
		{#each Array(size).fill(0) as _, x}
		<div class="cell-line">
			{#each Array(size).fill(0) as _, y}
			<div class="cell" style="width: {100 / size}%"
				data-x={x}
				data-y={y}
			>
				{#if !loaded}
				<div class="emoji" style="font-size: {puzzleFontSize};">{emojis[Math.floor(Math.random() * emojis.length)]}</div>
				{/if}
				{#if solved && x === thumbX && y === thumbY}
				<div class="emoji" style="font-size: {puzzleFontSize};">👏</div>
				{/if}
			</div>
			{/each}
		</div>
		{/each}
		{#each cells as cell}
		<div class="cell-in"
			style="
				top: {(cell.x + .02) * 100 / size}%;
				left: {(cell.y + .02) * 100 / size}%;
				width: {96 / size}%;
				font-size: {puzzleFontSize};
			">
			{cell.value}
		</div>
		{/each}
	</div>
	<div class="inputs">
		{#if !loaded}
		<label>
			Puzzle:
			<input type="text" bind:value={puzzleStr} />
		</label>
		<label>
			Solution:
			<input type="text" bind:value={solutionStr} />
		</label>
		<button class="apply-btn" on:click={() => {
			errorMsg = '';

			if (!puzzleStr.length || !solutionStr.length) {
				errorMsg = 'Please fill in both fields';
				return;
			}

			const puzzle = puzzleStr.split(' ').filter(x => x.length);
			solution = solutionStr.split(' ').filter(x => x.length);

			puzzleStr = puzzle.join(' ');
			solutionStr = solution.join(' ')

			const puzzleSize = isSqrd(puzzle.length);
			if (puzzleSize < 0) {
				errorMsg = 'Puzzle should be n x n size (n > 1)';
				return;
			}

			clearInterval(movingInterval);
			cells = puzzle.map((value, i) => {
				if (value === '0')
					return
				const x = Math.floor(i / puzzleSize);
				const y = i % puzzleSize;
				return { value, x, y }
			}).filter(x => x);
			size = puzzleSize;
			loaded = true;
			initialState = cells;
			solutionState = createSnail(size)
		}}>Apply</button>
		{:else}
		<div class="solution-wrapper">
			<h3 class="solution-title">Solution</h3>
			<div class="solution">
				{#each solution as step, i}
				<p class="step" style="{ i == solutionIndex ? "background: var(--main); color: #fff;" : "" }">{step}</p>
				{/each}
			</div>
		</div>
		<Player 
			{playing}
			on:play={() => {
				movingInterval = setInterval(() => {
					if (++solutionIndex>= solution.length) {
						clearInterval(movingInterval);
						playing = false;
						compareResult();
						solved = true;
						[thumbX, thumbY] = getMissingIndex(cells);
						return;
					}

					const [dx, dy] = directions[solution[solutionIndex]];
					const [emptyX, emptyY] = getMissingIndex(cells);

					let nextPos = getNextPos(emptyX, emptyY, dx, dy);
					if (nextPos[0] < 0) {
						clearInterval(movingInterval);
						cells = initialState;
						errorMsg = `Solution is not valid (step ${solutionIndex})`
						return;
					}
					let next = cells.find(cell => cell.x === nextPos[0] && cell.y === nextPos[1]);
					next.x = emptyX;
					next.y = emptyY;
					cells = cells;
				}, 400)
			}}
			on:pause={() => clearInterval(movingInterval)}
			on:stop={() => {
				clearInterval(movingInterval);
				cells = initialState;
				solutionIndex = -1;
				playing = false;
				solved = false;
				solveMsg = '';
				errorMsg = '';
			}}/>
		{/if}
		{#if errorMsg}
		<p class="msg" style="color: red; font-size: .9rem;">{errorMsg}</p>
		{/if}
		{#if solveMsg}
		<p class="msg" style="color: green; font-size: .9rem;">{solveMsg}</p>
		{/if}
	</div>
</main>

<style>
	* {
		user-select: none;
		font-family: Podkova;
	}

	main {
		width: 100vw;
		height: 100vh;
		background: #fef3c7;
		display: flex;
		justify-content: center;
		align-items: center;
		text-align: center;
		margin: 0;
		gap: 1rem;
	}

	.cell-container {
		position: relative;
		display: flex;
		flex-direction: column;
		min-height: 24rem;
		height: 80%;
		aspect-ratio: 1 / 1;
		border: 2px solid #78350f;
		border-radius: .2rem;

	}

	.cell-line {
		display: flex;
	}

	.cell {
		position: relative;
		width: 20%;
		aspect-ratio: 1 / 1;
		background: #f7fee7;
		border: 1px solid #451a03;
	}

	.cell-in {
		position: absolute;
		aspect-ratio: 1 / 1;
		background: var(--main);
		color: white;
		display: flex;
		justify-content: center;
		align-items: center;
		border-radius: 0.2rem;
		transition: .2s;
	}
	
	.inputs {
		display: flex;
		flex-direction: column;
		justify-content: center;
		align-items: center;
		min-width: 18rem;
		width: 18rem;
		height: 80%;
		border: 2px solid #451a03;
		border-radius: .2rem;
		gap: .5rem;
		background: #fffbeb;
		padding: 1rem;
	}

	.apply-btn {
		width: 5rem;
		padding: 0.5rem;
		background: var(--main);
		color: white;
		cursor: pointer;
		border: none;
	}

	.solution-wrapper {
		display: flex;
		flex-direction: column;
		justify-content: center;
		align-items: center;
	}

	.solution-title {
		margin: .2rem;
		padding: .2rem 1.2rem;
		border: 2px solid #fff;
		background: var(--main);
		color: white;
		border-radius: .1rem;
	}	

	.solution {
		padding: .3rem;
		display: grid;
		grid-template-columns: repeat(8, 1fr);
		gap: .2em;
		row-gap: .2em;
		border: 1px solid #451a03;
		border-radius: .2rem;
	}

	.step {
		padding: .2rem .3rem;
		background: #fff;
		border: 1px solid #451a03;
		margin: 0;
	}

	.emoji {
		position: absolute;
		display: flex;
		justify-content: center;
		align-items: center;
		width: 100%;
		height: 100%;

	}

	@media (max-width: 820px) {
		.inputs {
			display: none;
		}
	}
</style>