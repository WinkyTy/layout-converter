# Railway Deployment Guide for Layout Converter Bot

## 🚀 Quick Deploy to Railway

### Step 1: Create Telegram Bot
1. Message [@BotFather](https://t.me/botfather) on Telegram
2. Send `/newbot` command
3. Follow instructions to create your bot
4. **Save the bot token** (you'll need it for Railway)

### Step 2: Deploy to Railway
1. Go to [Railway.app](https://railway.app)
2. Sign up/Login with GitHub
3. Click "New Project"
4. Select "Deploy from GitHub repo"
5. Choose your `layout-converter` repository
6. Railway will automatically detect the Dockerfile

### Step 3: Configure Environment Variables
1. In your Railway project dashboard
2. Go to "Variables" tab
3. Add environment variable:
   - **Name**: `TELEGRAM_BOT_TOKEN`
   - **Value**: Your bot token from Step 1

### Step 4: Deploy
1. Railway will automatically build and deploy
2. Wait for build to complete (takes 2-3 minutes)
3. Check logs for any errors

## 🔧 Manual Deployment Steps

### Option A: Using Railway CLI
```bash
# Install Railway CLI
npm install -g @railway/cli

# Login to Railway
railway login

# Link to your project
railway link

# Set environment variable
railway variables set TELEGRAM_BOT_TOKEN=your_bot_token_here

# Deploy
railway up
```

### Option B: Using GitHub Integration
1. Connect your GitHub repository to Railway
2. Railway will automatically deploy on every push
3. Set environment variables in Railway dashboard

## 📊 Monitoring Your Bot

### Check Deployment Status
- Go to Railway dashboard
- Check "Deployments" tab
- Look for green checkmark ✅

### View Logs
- Click on your deployment
- Go to "Logs" tab
- Look for bot startup messages

### Test Your Bot
1. Find your bot on Telegram
2. Send `/start` command
3. Try converting some text

## 🐛 Troubleshooting

### Common Issues

**1. Build Fails**
- Check Railway logs for C++ compilation errors
- Ensure Dockerfile is in root directory
- Verify CMake and build tools are available

**2. Bot Not Responding**
- Check if `TELEGRAM_BOT_TOKEN` is set correctly
- Look for bot startup messages in logs
- Verify bot is not blocked by Telegram

**3. Library Not Found**
- Bot will run in fallback mode
- Check logs for "C++ library not found" warning
- Fallback mode still works for basic conversions

**4. Environment Variables**
- Ensure `TELEGRAM_BOT_TOKEN` is set
- Check variable name spelling
- Redeploy after changing variables

### Debug Commands
```bash
# Check bot health locally
cd telegram_bot
python3 health_check.py

# Test bot locally (requires token)
export TELEGRAM_BOT_TOKEN=your_token
python3 layout_converter_bot.py
```

## 🔄 Updating Your Bot

### Automatic Updates
- Push changes to GitHub
- Railway will automatically redeploy
- No manual intervention needed

### Manual Updates
```bash
# Force redeploy
railway up

# Check deployment status
railway status
```

## 📈 Scaling

### Free Tier
- 500 hours/month
- 1GB RAM
- Shared CPU

### Paid Plans
- Unlimited hours
- More RAM and CPU
- Custom domains

## 🔒 Security

### Environment Variables
- Never commit bot tokens to git
- Use Railway's secure variable storage
- Rotate tokens regularly

### Bot Security
- Don't share your bot token
- Use bot privacy mode if needed
- Monitor bot usage

## 📞 Support

### Railway Support
- [Railway Documentation](https://docs.railway.app)
- [Railway Discord](https://discord.gg/railway)

### Bot Issues
- Check Telegram Bot API documentation
- Review bot logs in Railway dashboard
- Test locally before deploying

## 🎉 Success Checklist

- [ ] Bot responds to `/start`
- [ ] Bot converts text correctly
- [ ] Bot handles errors gracefully
- [ ] Logs show successful startup
- [ ] Environment variables are set
- [ ] Build completes without errors

Your bot should now be live and ready for user testing! 🚀 