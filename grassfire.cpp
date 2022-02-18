from _future_ import division
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from tkinter import *
from tkinter import messagebox
import numpy as np
import random
import math

PI = math.pi

class Grassfire:

    START = 0
    DEST = -1
    UNVIS = -2
    OBST = -3
    PATH = -4

    COLOR_START = np.array([0.75, 0.75, 0])
    COLOR_DEST = np.array([0.75, 0, 0.75])
    COLOR_UNVIS = np.array([1, 1, 1])
    COLOR_VIS = np.array([0, 0.5, 1])
    COLOR_OBST = np.array([0, 0, 0])
    COLOR_PATH = np.array([1, 1, 0])

    def random_grid(self, rows=16, cols=16, obstacleProb=0.3):
        obstacleGrid = np.random.random_sample((rows, cols))
        grid = Grassfire.UNVIS * np.ones((rows, cols), dtype=np.int)
        grid[obstacleGrid <= obstacleProb] = self.OBST

        self.set_start_dest(grid)
        return grid

    def set_start_dest(self, grid):
        (rows, cols) = grid.shape

        grid[grid == Grassfire.START] = Grassfire.UNVIS
        grid[grid == Grassfire.DEST] = Grassfire.UNVIS

        validStartCell = False
        while not validStartCell:
            startIndex = random.randint(0, rows * cols - 1)
            startIndices = np.unravel_index(startIndex, (rows, cols))
            if grid[startIndices] != Grassfire.OBST:
                validStartCell = True
                grid[startIndices] = Grassfire.START

        validDestCell = False
        while not validDestCell:
            destIndex = random.randint(0, rows * cols - 1)
            destIndices = np.unravel_index(destIndex, (rows, cols))
            if grid[destIndices] != Grassfire.START and grid[destIndices] != Grassfire.OBST:
                validDestCell = True
                grid[destIndices] = Grassfire.DEST

    def color_grid(self, grid):
        (rows, cols) = grid.shape
        colorGrid = np.zeros((rows, cols, 3), dtype=np.float)

        colorGrid[grid == Grassfire.OBST, 🙂 = Grassfire.COLOR_OBST
        colorGrid[grid == Grassfire.UNVIS, 🙂 = Grassfire.COLOR_UNVIS
        colorGrid[grid == Grassfire.START, 🙂 = Grassfire.COLOR_START
        colorGrid[grid == Grassfire.DEST, 🙂 = Grassfire.COLOR_DEST
        colorGrid[grid > Grassfire.START, 🙂 = Grassfire.COLOR_VIS
        colorGrid[grid == Grassfire.PATH, 🙂 = Grassfire.COLOR_PATH
        return colorGrid

    def reset_grid(self, grid):
        cellsToReset = ~((grid == Grassfire.OBST) + (grid == Grassfire.START)
            + (grid == Grassfire.DEST))
        grid[cellsToReset] = Grassfire.UNVIS

    def _check_adjacent(self, grid, cell, currentDepth):
        (rows, cols) = grid.shape

        numCellsUpdated = 0

        for i in range(4):
            rowToCheck = cell[0] + int(math.sin((PI/2) * i))
            colToCheck = cell[1] + int(math.cos((PI/2) * i))

            if not (0 <= rowToCheck < rows and 0 <= colToCheck < cols):
                continue
            elif grid[rowToCheck, colToCheck] == Grassfire.DEST:
                return Grassfire.DEST
            elif (grid[rowToCheck, colToCheck] == Grassfire.UNVIS
                or grid[rowToCheck, colToCheck] > currentDepth + 1):
                grid[rowToCheck, colToCheck] = currentDepth + 1
                numCellsUpdated += 1
        return numCellsUpdated

    def _backtrack(self, grid, cell, currentDepth):
        (rows, cols) = grid.shape

        for i in range(4):
            rowToCheck = cell[0] + int(math.sin((PI/2) * i))
            colToCheck = cell[1] + int(math.cos((PI/2) * i))

            if not (0 <= rowToCheck < rows and 0 <= colToCheck < cols):
                continue
            elif grid[rowToCheck, colToCheck] == currentDepth:
                nextCell = (rowToCheck, colToCheck)
                grid[nextCell] = Grassfire.PATH
                return nextCell

    def find_path(self, grid):
        nonlocalDict = {'grid': grid}
        def find_path_generator():
            grid = nonlocalDict['grid']
            depth = 0
            destFound = False
            cellsExhausted = False

            while (not destFound) and (not cellsExhausted):
                numCellsModified = 0
                depthIndices = np.where(grid == depth)
                matchingCells = list(zip(depthIndices[0], depthIndices[1]))

                for cell in matchingCells:
                    adjacentVal = self._check_adjacent(grid, cell, depth)
                    if adjacentVal == Grassfire.DEST:
                        destFound = True
                        break
                    else:
                        numCellsModified += adjacentVal

                if numCellsModified == 0:
                    cellsExhausted = True
                elif not destFound:
                    depth += 1
                yield

            if destFound:
                destCell = np.where(grid == Grassfire.DEST)
                backtrackCell = (destCell[0].item(), destCell[1].item())
                while depth > 0:
                    # Work backwards until return to start cell.
                    nextCell = self._backtrack(grid, backtrackCell, depth)
                    backtrackCell = nextCell
                    depth -= 1
                    yield
        return find_path_generator

PI = math.pi

root = Tk()
root.geometry("150x100")

x_label = Label(root, text="row").grid(row=0,column=0)
x=Entry(root,width=15)
x.grid(row=0,column=1)
y_label = Label(root, text="column").grid(row=1,column=0)
y=Entry(root,width=15)
y.grid(row=1,column=1)

def get_grid():
    global r
    global c
    r = x.get()
    c = y.get()
    messagebox.showinfo("Grid Generated", "You can now start!")

enter_button = Button(root, text="Enter Grid", command=get_grid)
enter_button.grid(row=2,column=1)
exit_button = Button(root, text="Start",command=root.destroy)
exit_button.grid(row=3,column=1)

root.mainloop()

rows = int(r)
cols = int(c)
obstProb = 0.3

Grassfire = Grassfire()
grid = Grassfire.random_grid(rows=rows, cols=cols, obstacleProb=obstProb)
colorGrid = Grassfire.color_grid(grid)

fig = plt.figure()
gridPlot = plt.imshow(colorGrid, interpolation='nearest')
ax = gridPlot._axes
obstText = ax.annotate('', (0.15, 0.01), xycoords='figure fraction')
colText = ax.annotate('', (0.15, 0.04), xycoords='figure fraction')
rowText = ax.annotate('', (0.15, 0.07), xycoords='figure fraction')
ax.grid(visible=True, ls='solid', color='k', lw=1.5)
ax.set_xticklabels([])
ax.set_yticklabels([])

def update_annotations(rows, cols, obstProb):
    obstText.set_text('Obstacle density: {:.0f}%'.format(obstProb * 100))
    colText.set_text('Rows: {:d}'.format(rows))
    rowText.set_text('Columns: {:d}'.format(cols))

def set_axis_properties(rows, cols):
    ax.set_xlim((0, cols))
    ax.set_ylim((rows, 0))
    ax.set_xticks(np.arange(0, cols+1, 1))
    ax.set_yticks(np.arange(0, rows+1, 1))
    gridPlot.set_extent([0, cols, 0, rows])

set_axis_properties(rows, cols)
update_annotations(rows, cols, obstProb)

fig.canvas.mpl_disconnect(fig.canvas.manager.key_press_handler_id)

def on_key_press(event):
    global grid, rows, cols, obstProb
    if event.key == 'enter':
        ani._stop()
        exit()
    elif event.key == 'shift':
        Grassfire.set_start_dest(grid)
        Grassfire.reset_grid(grid)
        ani.frame_seq = ani.new_frame_seq()
    elif event.key == 'control':
        grid = Grassfire.random_grid(rows=rows, cols=cols, obstacleProb=obstProb)
        set_axis_properties(rows, cols)
        ani._iter_gen = Grassfire.find_path(grid)
    elif event.key == 'right':
        rows += 1
        update_annotations(rows, cols, obstProb)
    elif event.key == 'left' and rows > 1:
        rows -= 1
        update_annotations(rows, cols, obstProb)
    elif event.key == 'up':
        cols += 1
        update_annotations(rows, cols, obstProb)
    elif event.key == 'down' and cols > 1:
        cols -= 1
        update_annotations(rows, cols, obstProb)
    elif event.key.isdigit():
        obstProb = int(event.key) / 10
        update_annotations(rows, cols, obstProb)
fig.canvas.mpl_connect('key_press_event', on_key_press)

def update_anim(dummyFrameArgument):
    colorGrid = Grassfire.color_grid(grid)
    gridPlot.set_data(colorGrid)

def init_anim():
    Grassfire.reset_grid(grid)
    colorGrid = Grassfire.color_grid(grid)
    gridPlot.set_data(colorGrid)

ani = animation.FuncAnimation(fig, update_anim,
    init_func=init_anim, frames=Grassfire.find_path(grid),
    repeat=True, interval=150)

plt.ion()
plt.show(block=True)