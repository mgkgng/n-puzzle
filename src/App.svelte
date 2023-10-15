<script>
	import { onMount } from 'svelte';
	import Player from './Player.svelte';
    import { fly } from 'svelte/transition';

	let puzzleSize;
	let puzzle;
	let puzzleRes;
	let errorMsg = '';
	let solved = false;
	let thumbX, thumbY
	let cells;
	let initialState;
	let solutionState;
	let puzzleStr = '';
	let puzzleFontSize;
	$: puzzleFontSize = `${Math.floor(20 / puzzleSize)}rem`

	let loaded = false;

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

	let emojis = ['🧠', '🐊', '🎃', '👁️', '​💀​', '🫀', '​🐐', '​🔥', '⭐', '​👾', '​​🪲', '​🐲', '🫐', '​🍥', '​🌚', '​💣', '🌸']
	emojis = emojis.sort((a, b) => 0.5 - Math.random());

	let state = 0;
	let algorithmChoice = 0;
	let heuristicsChoice = 0;
	let strategyChoice = 0;

	let WASM_SRC_DIR = 'wasm/n_puzzle_wasm.js'
	let wasmOutput = []
	let wasmArgs = []

	let selectedFile = null;
    let fileContents = '';
	let fileInputElem;

	function back() {
		state--;
		errorMsg = "";
	}

	function isSqrd(n) {
		if (n < 4) return false; // Exclude numbers smaller than 4

		const sqrt = Math.sqrt(n);
		return (sqrt === Math.floor(sqrt)) ? sqrt : -1;
	}

	function getMissingIndex(cells) {
		const helper = new Set(Array.from({ length: puzzleSize * puzzleSize }, (_, i) => i));
		cells.forEach(cell => helper.delete(cell.x * puzzleSize + cell.y));
		const left = helper.values().next().value;
		return [Math.floor(left / puzzleSize), left % puzzleSize];
	}

	function getNeighbors(x, y) {
		let res = [];
		if (x > 0) res.push(cells.find(cell => cell.x === x - 1 && cell.y === y));
		if (x < puzzleSize - 1) res.push(cells.find(cell => cell.x === x + 1 && cell.y === y));
		if (y > 0) res.push(cells.find(cell => cell.x === x && cell.y === y - 1));
		if (y < puzzleSize - 1) res.push(cells.find(cell => cell.x === x && cell.y === y + 1));
		return res.filter(cell => cell.x != prevPos[0] && cell.y != prevPos[1]);
	}

	function getNextPos(x, y, dx, dy) {
		if (x + dx < 0 || x + dx >= puzzleSize || y + dy < 0 || y + dy >= puzzleSize)
			return [-1, -1];
		return [x + dx, y + dy];
	}

	function createSnail(size) {
		let res = Array.from({ length: size }, () => Array.from({ length: size }, () => 0));

		const getValue = (val) => val === size * size ? 0 : val;
		let val = 1;

		let rowStart = 0, rowEnd = size - 1;
		let colStart = 0, colEnd = size - 1;

		while (rowStart <= rowEnd && colStart <= colEnd) {
			for (let j = colStart; j <= colEnd; ++j)
				res[rowStart][j] = getValue(val++);
			++rowStart;

			for (let i = rowStart; i <= rowEnd; ++i)
				res[i][colEnd] = getValue(val++);
			--colEnd;

			for (let j = colEnd; j >= colStart; --j)
				res[rowEnd][j] = getValue(val++);
			--rowEnd;

			for (let i = rowEnd; i >= rowStart; --i)
				res[i][colStart] = getValue(val++);
			++colStart;
		}
		return res;
	}

	function verifyPuzzleFile(size, arr) {
		if (size < 2 || arr.length < 4) {
			errorMsg = 'Puzzle should be n x n size (n > 1)';
			return false;
		}
		if (size * size !== arr.length) {
			errorMsg = 'Puzzle size does not match';
			return false;
		}
		return true;
	}

	function parsePuzzleFile(str) {
		let res = '';
		let size = 0;
		let lines = str.split('\n');
		for (let line of lines) {
			if (line.startsWith('#'))
				continue;
			else if (!size) {
				size = parseInt(line);
				continue;
			} else {
				res = [...res, ...line.split(' ').filter(x => x.length)]
			}
		}
		return verifyPuzzleFile(size, res) ? res.join(' ') : null;
	}

	function createPuzzle(size) {
		let solution = createSnail(size);

		let zeroPos;
		for (let i = 0; i < size; i++) {
			for (let j = 0; j < size; j++) {
				if (solution[i][j] === 0) {
					zeroPos = [i, j];
					break;
				}
			}
		}

		const dirs = Object.values(directions);
		for (let i = 0; i < 3000; ++i) {
			let dir = dirs[Math.floor(Math.random() * dirs.length)];
			let nextPos = getNextPos(zeroPos[0], zeroPos[1], dir[0], dir[1]);
			if (nextPos[0] < 0) continue;
			[solution[zeroPos[0]][zeroPos[1]], solution[nextPos[0]][nextPos[1]]] = [solution[nextPos[0]][nextPos[1]], solution[zeroPos[0]][zeroPos[1]]];
			zeroPos = nextPos;
		}

		return solution;
	}

	function compareResult() {
		for (let cell of cells) {
			if (parseInt(cell.value) !== solutionState[cell.x][cell.y]) {
				errorMsg = 'Solution is not valid (Wrong Result)'
				return
			}
		}
	}

	function parseOutputs(outputs) {
		let res = {};
		for (let i = 1; i < outputs.length; i++) {
			let [key, value] = outputs[i].split('#');
			res[key] = value;
		}
		return res;
	}

	function loadWasm() {
		return new Promise((resolve, reject) => {
			if (window.Module) {
				resolve(window.Module); // if already loaded
				return;
			}

			window.Module = {
				onRuntimeInitialized: () => {
					resolve(window.Module);
				},
				print: (text) => {
					wasmOutput = [...wasmOutput, text]
				},
			}

			const script = document.createElement('script');
			script.src = WASM_SRC_DIR;
			script.onerror = reject;
			document.head.appendChild(script);
		});
	}

	function resetInitialAnimation() {
		puzzleSize = 3;
		cells = [
			{ value: 'N', x: 0, y: 0 },
			{ value: '-', x: 0, y: 1 },
			{ value: 'P', x: 1, y: 0 },
			{ value: 'U', x: 1, y: 1 },
			{ value: 'Z', x: 1, y: 2 },
			{ value: 'Z', x: 2, y: 0 },
			{ value: 'L', x: 2, y: 1 },
			{ value: 'E', x: 2, y: 2 },		
		]
		movingInterval = setInterval(() => {
			const [emptyX, emptyY] = getMissingIndex(cells);
			const neighbors = getNeighbors(emptyX, emptyY);
			let picked = neighbors[Math.floor(Math.random() * neighbors.length)];
			picked.x = emptyX;
			picked.y = emptyY;
			prevPos = [emptyX, emptyY];
			cells = cells
		}, 800)
	}

	onMount(() => {
        document.documentElement.style.setProperty('--main', '#ff3e00');
		resetInitialAnimation();
	})
</script>

<header>
	<div class="title">
		<div class="title-wrapper">
			<h1>N-Puzzle Solver</h1>
			<img style="height: 50%; transform: translateY(-5%) rotate(10deg);" src="icons/white_puzzle.svg" alt="puzzle">
		</div>
	</div>

</header>
<main>
	<div class="cell-container">
		{#each Array(puzzleSize).fill(0) as _, x}
		<div class="cell-line">
			{#each Array(puzzleSize).fill(0) as _, y}
			<div class="cell" style="width: {100 / puzzleSize}%"
				data-x={x}
				data-y={y}
			>
				{#if !loaded}
				<div class="emoji" style="font-size: {puzzleFontSize};">{emojis[x * puzzleSize + y]}</div>
				{/if}
				{#if solved && x === thumbX && y === thumbY}
				<div class="emoji" style="font-size: {puzzleFontSize};">👏</div>
				{/if}
			</div>
			{/each}
		</div>
		{/each}
		{#if cells}
		{#each cells as cell}
		<div class="cell-in"
			style="
				top: {(cell.x + .02) * 100 / puzzleSize}%;
				left: {(cell.y + .02) * 100 / puzzleSize}%;
				width: {96 / puzzleSize}%;
				font-size: {puzzleFontSize};
			">
			{cell.value}
		</div>
		{/each}
		{/if}
	</div>
	<div class="state-manager">
		{#if state == 0}
		<button on:click={() => state++}>Start!</button>
		{:else if state == 1}
		<div in:fly={{x: -100}} style="transform: translateY(-25%);">
			<h2>Puzzle?</h2>
			<div style="display: flex; gap: .2em;">
				<button on:click={() => state += 2}>generate</button>
				<button on:click={() => state++}>load</button>
			</div>
		</div>
		{:else if state == 2}
		<button class="back-btn" on:click={back}>&lt;</button>
		<div in:fly={{x: -100}} style="display: flex; flex-direction: column; gap: .4rem; justify-items: center; align-items: center;">
			<input hidden type="file" accept=".txt" on:change={(e) => {
				const files = e.target.files;
				selectedFile = files.length === 0 ? null : files[0];
			}} 
				bind:this={fileInputElem}
			/>
			<button style="background: none; border: 2px solid var(--main); color: var(--main);"on:click={() => fileInputElem.click()}>choose file (.txt)</button>
			<button style="width: 5em;" on:click={() => {
				errorMsg = '';

				if (!selectedFile) {
					errorMsg = "No file selected.";
					return;
				}

				const reader = new FileReader();
				reader.onload = (e) => {
					fileContents = e.target.result;
					const parsed = parsePuzzleFile(fileContents);
					if (!parsed)
						return;
					puzzleStr = parsed;
					puzzle = puzzleStr.split(' ').filter(x => x.length).map(x => parseInt(x));
					state++;
				};
				reader.onerror = (event) => {
					console.error("File reading error: ", event);
					errorMsg = 'File reading error';
				};
				reader.readAsText(selectedFile);
			}}>Next</button>
			{#if errorMsg}
			<p class="msg" style="color: red; font-size: .9rem; top: 55%;">{errorMsg}</p>
			{/if}
		</div>
		{:else if state == 3}
		<button class="back-btn" on:click={() => {
			state -= 2;
			puzzleStr = '';
			puzzle = null;
			fileContents = '';
			errorMsg = '';
			selectedFile = null;
		}}>&lt;</button>
		<div in:fly={{x: -100}} style="display: flex; flex-direction: column; gap: .3em; justify-content: center; align-items: center">
			<div>
				<h3 style="margin: 0.5em;">Choose your algorithm:</h3>
				<div>
					<input type="radio" id="a" bind:group={algorithmChoice} value={1}>
					<label class="radio-label" for="a">A*</label>
					<input type="radio" id="ida" bind:group={algorithmChoice} value={2}>
					<label class="radio-label" for="ida" on:click={() => {
						if (strategyChoice != 1)
							strategyChoice = 0;
					}}>Iterative Deepening A*</label>
				</div>
			</div>
			<div>
				<h3 style="margin: 0.5em;">Choose your heuristics:</h3>
				<div class="heuristics">
					<input type="radio" id="mht" bind:group={heuristicsChoice} value={1}>
					<label class="radio-label" for="mht">Manhattan</label>
					<input type="radio" id="euc" bind:group={heuristicsChoice} value={2}>
					<label class="radio-label" for="euc">Euclidean</label>
					<input type="radio" id="ham" bind:group={heuristicsChoice} value={3}>
					<label class="radio-label" for="ham">Hamming</label>
					<input type="radio" id="lico" bind:group={heuristicsChoice} value={4}>
					<label class="radio-label" for="lico">Linear Conflict</label>
				</div>
			</div>
			<div>
				<h3 style="margin: 0.5em;">Choose your search strategy:</h3>
				<div class="strategy">
					<input type="radio" id="std" bind:group={strategyChoice} value={1}>
					<label class="radio-label" for="std">Standard</label>
					<input type="radio" id="uc" bind:group={strategyChoice} value={2} disabled={algorithmChoice === 2}>
					<label class="radio-label {algorithmChoice === 2 ? "disabled" : ""}" for="uc">Uniform-Cost</label>
					<input type="radio" id="gdy" bind:group={strategyChoice} value={3} disabled={algorithmChoice === 2}>
					<label class="radio-label {algorithmChoice === 2 ? "disabled" : ""}" for="gdy">Greedy</label>
				</div>
			</div>
			<button style="margin-top: 1.2em; width: 6rem;" on:click={() => {
				errorMsg = ""
				if (algorithmChoice == 0) {
					errorMsg = "You must choose an algorithm"
					return
				}
				if (heuristicsChoice == 0) {
					errorMsg = "You must choose a heuristics"
					return
				}
				if (strategyChoice == 0) {
					errorMsg = "You must choose a strategy"
					return
				}

				state++
				if (puzzleStr == '') {
					puzzleSize = 3
					puzzle = createPuzzle(puzzleSize).flat()
					puzzleStr = puzzle.join(' ')
				} else {
					puzzleSize = Math.sqrt(puzzle.length)
				}

				clearInterval(movingInterval)
				cells = puzzle.map((value, i) => {
					if (value === 0)
						return
					const x = Math.floor(i / puzzleSize);
					const y = i % puzzleSize;
					return { value, x, y }
				}).filter(x => x);
				loaded = true;
				initialState = JSON.parse(JSON.stringify(cells));
				solutionState = createSnail(puzzleSize)

				setTimeout(() => {
					wasmArgs = [
						algorithmChoice.toString(),
						heuristicsChoice.toString(),
						strategyChoice.toString(),
						puzzleStr
					]

					loadWasm().then(module => {
						module.callMain(wasmArgs);
						if (wasmOutput.length > 0 && wasmOutput[0] == 'SUCCESS') {
							puzzleRes = parseOutputs(wasmOutput)
							state++;
						} else {
							errorMsg = wasmOutput[1].split('#')[1];
							state += 2;
						}
					});
				}, 200);

			}}>Next</button>
			{#if errorMsg}
			<p class="msg" style="color: red; font-size: .9rem;">{errorMsg}</p>
			{/if}
		</div>
		{:else if state == 4}
		<div in:fly={{x: -100}} style="background: white; padding: 1.4rem 4.4rem; padding-bottom: .8rem; border-radius: .2em; display: flex; flex-direction: column; justify-content: center; align-items: center;">
			<div class="lds-ring"><div></div><div></div><div></div><div></div></div>
			<p style="color: var(--main); margin: .6rem;">solving...</p>
		</div>
		{:else if state == 5}
			<div class="summary">
				<h3 class="solution-title">Summary</h3>
				<div class="summary-wrapper">
					<div class="summary-line">
						<p style="font-size: 14px;">Total visited states:</p>
						<p>{puzzleRes.TSV}</p>
					</div>
					<div class="summary-line">
						<p style="font-size: 14px;">Max number of states:</p>
						<p>{puzzleRes.MNS}</p>
					</div>
					<div class="summary-line">
						<p style="font-size: 14px;">Time elapsed:</p>
						<p>{puzzleRes.TME}ms</p>
					</div>
				</div>
			</div>
			<div class="solution-wrapper">
				<h3 class="solution-title">Solution</h3>
				<div class="solution">
					{#if puzzleRes?.SOL}
						{#each puzzleRes.SOL as step, i}
						<p class="step" style="{ i == solutionIndex ? "background: var(--main); color: #fff;" : "" }">{step}</p>
						{/each}
					{/if}
				</div>
			</div>
			<Player 
				bind:playing={playing}
				on:play={() => {
					movingInterval = setInterval(() => {
						if (++solutionIndex>= puzzleRes.SOL.length) {
							clearInterval(movingInterval);
							playing = false;
							compareResult();
							solved = true;
							[thumbX, thumbY] = getMissingIndex(cells);
							return;
						}
	
						const [dx, dy] = directions[puzzleRes.SOL[solutionIndex]];
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
				on:pause={() => {
					clearInterval(movingInterval);
				}}
				on:stop={() => {
					clearInterval(movingInterval);
					cells = initialState;
					playing = false;
					solutionIndex = -1;
					solved = false;
				}}/>
		{:else}
		<div>
			<button class="back-btn" on:click={() => {
				state = 1;
				puzzleStr = '';
				puzzle = null;
				loaded = false;
				resetInitialAnimation();
			}}>&lt;</button>
			<p style="color: red;">{errorMsg}</p>
		</div>
		{/if}
	</div>
</main>

<style>
	* {
		user-select: none;
		font-family: Podkova;
	}

	header {
		position: absolute;
		top: 10%;
		width: 100%;
		display: flex;
		justify-content: center;
		align-items: center;
	}

	header h1 {
		color: white;
		font-size: 2.2rem;
		font-family: 'Podkova-ExtraBold';
		padding: 0;
		margin: 0;
	}

	.title-wrapper {
		display: flex;
		justify-content: center;
		align-items: center;
		gap: .5rem;

		background: var(--main);
		height: 100%;

		border-radius: .1rem;
		height: 4rem;

	}

	.title {
		width: 25rem;
		/* border: 5px dashed var(--main); */
		border-radius: .2rem;
		background: white;
	}

	button {
		cursor: pointer;
		border-radius: .1rem;
		background: var(--main); 
		color: white; 
		border: none; 
		padding: 0.3rem 1rem;

	}

	button:hover {
		filter: brightness(90%);
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
		height: 60%;
		transform: translateY(10%);
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

	.state-manager {
		position: relative;
		display: flex;
		flex-direction: column;
		justify-content: center;
		align-items: center;
		min-width: 18rem;
		width: 18rem;
		height: 60%;
		transform: translateY(10%);
		background: #fffbeb;
		padding: 1rem;
		overflow-y: auto;
	}

	.back-btn {
		position: absolute;
		top: 0.2rem; 
		left: 0.2rem;
	}
	
	.summary {
		display: flex;
		flex-direction: column;
		justify-content: center;
		align-items: center;
		width: 80%;
	}
	
	.summary-line {
		width: 100%;
		display: grid;
		grid-template-columns: 50% 50%;
	}
	.summary-line p, div {
		margin: 0;
	}
	.summary-line p {
		background: white;
	}

	.summary-wrapper {
		display: flex;
		flex-direction: column;
		border: 1px solid black;
		padding: .5rem;
		border-radius: .2rem;
		gap: .2em;
		width: 100%;
	}

	.solution-wrapper {
		display: flex;
		flex-direction: column;
		justify-content: center;
		align-items: center;
	}

	.solution-title {
		width: 5rem;
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

	input[type="radio"] {
		display: none;
	}

	.radio-label {
		border: 2px solid var(--main);
		border-radius: .2rem;
		background: white;
		padding: .2rem .5rem;
	}

	input[type="radio"]:checked + .radio-label {
		background: var(--main);
		color: white;
	}

	.heuristics {
		display: grid;
		grid-template-columns: repeat(2, 1fr);
		grid-gap: .2em;
	}

	.lds-ring {
		display: inline-block;
		position: relative;
		width: 40px;
		height: 40px;
	}
	.lds-ring div {
		box-sizing: border-box;
		display: block;
		position: absolute;
		width: 40px;
		height: 40px;
		/* margin: 8px; */
		border: 6px solid #FF5322;
		border-radius: 50%;
		animation: lds-ring 1.2s cubic-bezier(0.5, 0, 0.5, 1) infinite;
		border-color: #f86036 transparent transparent transparent;
	}
	.lds-ring div:nth-child(1) {
		animation-delay: -0.45s;
	}
	.lds-ring div:nth-child(2) {
		animation-delay: -0.3s;
	}
	.lds-ring div:nth-child(3) {
		animation-delay: -0.15s;
	}
	@keyframes lds-ring {
		0% {
			transform: rotate(0deg);
		}
		100% {
			transform: rotate(360deg);
		}
	}

	.msg {
		position: absolute;
		/* bottom: .1em; */
	}

	.disabled {
		background: #666;
		color: #aaa;
		border-color: #aaa;
	}


	@media (max-width: 820px) {
		.state-manager {
			display: none;
		}
	}
</style>