<script>
	import { onMount } from 'svelte';
	let size = 3;
	let errorMsg = '';
	let cells = Array(size).fill(Array(size).fill(2));
	let puzzleStr, solutionStr;
	let puzzleFontSize;
	$: puzzleFontSize = `${Math.floor(20 / size)}rem`

	function isSqrd(n) {
		if (n < 4) return false; // Exclude numbers smaller than 4

		const sqrt = Math.sqrt(n);
		return (sqrt === Math.floor(sqrt)) ? sqrt : -1;
	}

	onMount(() => {
        document.documentElement.style.setProperty('--main', '#ff3e00');
	})
</script>

<main>
	<div class="cell-container">
		{#each Array(size).fill(0) as _, i}
		<div class="cell-line">
			{#each Array(size).fill(0) as _, j}
			<div class="cell" style="width: {100 / size}%">
				{#if cells[i][j] !== '0'}
				<div class="cell-in" style="font-size: {puzzleFontSize};">{cells[i][j]}</div>
				{/if}	
			</div>
			{/each}
		</div>
		{/each}
	</div>
	<div class="inputs">
		<label>
			Puzzle:
			<input type="text" bind:value={puzzleStr} />
		</label>
		<label>
			Solution:
			<input type="text" bind:value={solutionStr} />
		</label>
		<button class="apply-btn" on:click={() => {
			if (!puzzleStr.length || !solutionStr.length) {
				errorMsg = 'Please fill in both fields';
				return;
			}

			console.log(puzzleStr, solutionStr)

			const puzzle = puzzleStr.split(' ').filter(x => x.length);
			const solution = solutionStr.split(' ').filter(x => x.length);

			puzzleStr = puzzle.join(' ');
			solutionStr = solution.join(' ')

			const puzzleSize = isSqrd(puzzle.length);
			if (puzzleSize < 0) {
				errorMsg = 'Puzzle should be n x n size (n > 1)';
				return;
			}

			cells = Array.from({ length: puzzleSize }, (_, i) => Array.from({ length: puzzleSize }, (_, j) => puzzle[i * puzzleSize + j]));
			size = puzzleSize;
		}}>Apply</button>
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
		max-width: 240px;
		margin: 0;
		gap: 1rem;
	}

	.cell-container {
		display: flex;
		flex-direction: column;
		height: 80%;
		aspect-ratio: 1 / 1;
		border: 1px solid #444;

	}

	.cell-line {
		display: flex;
	}

	.cell {
		position: relative;
		width: 20%;
		aspect-ratio: 1 / 1;
		background: #f7fee7;
		border: 1px solid #666;
	}

	.cell-in {
		position: absolute;
		width: 96%;
		aspect-ratio: 1 / 1;
		background: var(--main);
		top: 50%;
		left: 50%;
		transform: translate(-50%, -50%);
		color: white;
		display: flex;
		justify-content: center;
		align-items: center;
		border-radius: 0.2rem;
	}
	
	.inputs {
		display: flex;
		flex-direction: column;
		justify-content: center;
		align-items: center;
		width: 25%;
		height: 80%;
		border: 1px solid #666;
		gap: .5rem;
		background: #fffbeb;
	}

	.apply-btn {
		width: 5rem;
		padding: 0.5rem;
		background: var(--main);
		color: white;
		cursor: pointer;
		border: none;
	}

	@media (min-width: 640px) {
		main {
			max-width: none;
		}
	}
</style>