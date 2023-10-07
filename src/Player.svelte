<script>
    import { createEventDispatcher } from 'svelte';

    const dispatch = createEventDispatcher()
    
    export let playing
</script>

<div class="player">
    <button on:click={() => {
        dispatch((playing) ? 'pause' : 'play')
        playing = !playing
    }}>
        {#if !playing}
        <img src="icons/play.svg" alt="play">
        {:else}
        <img src="icons/pause.svg" alt="pause">
        {/if}
    </button>
    <button on:click={() => dispatch('stop')}>
        <img src="icons/stop.svg" alt="stop">
    </button>
</div>

<svelte:window
    on:keydown={(e) => {
        if (e.code === 'Space')
            dispatch('play')
        else if (e.code === 'ArrowLeft')
            dispatch('prev')
        else if (e.code === 'ArrowRight')
            dispatch('next')
        else if (e.code === 'ArrowUp')
            dispatch('first')
        else if (e.code === 'ArrowDown')
            dispatch('last')
        else if (e.code === 'Escape')
            dispatch('stop')
    }}
/>

<style>
    .player {
        display: flex;
        gap: .2rem;
        margin-top: .3rem;
    }

    button {
        background: #ff3e00;
        display: flex;
        justify-content: center;
        align-items: center;
        padding: .5rem;
        border: none;
        border-radius: .2rem;
        cursor: pointer;
    }

    button:hover {
        filter: brightness(.9);
    }

    img {
        width: 1.2rem;
        height: 1.2rem;

    }
    /* .player {
        @apply flex gap-2 w-full justify-center items-center;
    }
    .play-button {
        @apply w-12 h-12 bg-teal-400 p-2 rounded-md hover:brightness-110;
    } */
</style>

