import discord
from discord.ext import commands
import requests

intents = discord.Intents.default()
bot = commands.Bot(command_prefix='!', intents=intents)

@bot.event
async def on_ready():
    print(f'Logged in as {bot.user.name}')

@bot.command()
async def water(ctx, action: str):
    # IP and port
    response = requests.get(f'http://ESP8266_IP/water?action={action}')
    await ctx.send(f'Watering command sent: {response.text}')

# Bot Token
bot.run('BOT_TOKEN')
